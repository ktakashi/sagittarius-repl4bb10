// Application with the Sheet project template
#include "SagittariusRepl.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include <QProcess>
#include <QDebug>
#include <QFileInfo>
#include <QStringList>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "Configuration.hpp"

using namespace bb::cascades;

SagittariusRepl::SagittariusRepl(bb::cascades::Application *app)
: QObject(app), running_(false), error_(false)
{
	// create scene document from main.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	// create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();
	// set created root object as a scene
	app->setScene(root);

	qml->setContextProperty("repl", this);

	// initialise signals
	QObject::connect(&repl_, SIGNAL(stateChanged(QProcess::ProcessState)),
			this, SLOT(observe(QProcess::ProcessState)));
	QObject::connect(&socket_, SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(socketError(QAbstractSocket::SocketError)));
	QObject::connect(this, SIGNAL(finished()), this, SLOT(onFinished()));

	QObject::connect(this, SIGNAL(ready()), root, SLOT(replReady()));
	QObject::connect(this, SIGNAL(error()), root, SLOT(replError()));

	// TODO read from configuration file
	config_ = new ReplConfig();

	qml->setContextProperty("config", config_);
}

SagittariusRepl::~SagittariusRepl()
{
	delete config_;
	execute(":exit");
	repl_.close();
	socket_.close();
}

QString SagittariusRepl::execute(const QString &expr)
{
	if (!socket_.isValid()) return "socket error";
	// send :datum tag and expression as utf8
	socket_.write(":datum ");
	socket_.write(expr.toUtf8());
	// wait until it's sent
	socket_.waitForBytesWritten();

	flushProcess();

	// now read the resonse
	QString tag = readTag(true);

	// we only have :exit :values and :error
	if (tag == ":values") {
		int count = readValuesCount();
		QString buf;
		qDebug() << "received count:" << count;
		for (int i = 0; i < count; i++) {
			buf += readDatum();
			buf += "\n";
		}
		return buf;
	} else if (tag == ":error") {
		QString buf;
		buf += "error in " + readDatum() + "\n";
		buf += readDatum();
		return buf;
	} else if (tag == ":exit") {
		return "[exit]";
	} else {
		// TODO should we?
	}

	return "";
}

void SagittariusRepl::restart()
{
	execute(":exit");
	if (running_) {
		repl_.close();
	}
	if (socket_.isValid()) {
		socket_.close();
	}
	// TODO use mutex...
	for (;;) {
		if (!running_) break;
		sleep(1);
	}
	initRemoteREPL();
}

static bool sanity(const QString &name, bool checkExec = false)
{
	// sanity check
	QFileInfo info(name);
	QString msg(name);
	if (!info.exists()) {
		msg += " doesn't exist";
		goto err;
	}
	if (!info.isReadable()) {
		msg += " isn't readable";
		goto err;
	}
	if (checkExec && !info.isExecutable()) {
		msg += " isn't executable";
		goto err;
	}
	return true;

err:
	qDebug() << msg;
	return false;
}

#define MARK_FILE "data/ready"

bool SagittariusRepl::initRemoteREPL()
{
	//if (!sanity("app/native/invoke.sh")) return false;
	if (!sanity("app/native/lib/sash", true)) return false;
	if (!sanity("app/native/lib/libgcmt-dll.so")) return false;
	if (!sanity("app/native/lib/libsagittarius.so")) return false;

	// remove if exists
	QFile mark(MARK_FILE);
	if (mark.exists()) mark.remove();

	repl_.start("app/native/lib/sash",
			QStringList() << "-Dapp/native/modules"
			<< "-Lapp/native/scheme"
			<< "app/native/repl.scm"
			<< QString::number(config_->port()));
	//repl_.start("/bin/sh", QStringList() << "app/native/invoke.sh");
	return true;
}

QString SagittariusRepl::readTag(bool wait)
{
	return readThing(' ', false, wait);
}

int SagittariusRepl::readValuesCount()
{
	QString count = readTag();
	return count.toInt();
}

QString SagittariusRepl::readDatum()
{
	char c;
	if (!socket_.getChar(&c)) return "";
	// check sanity
	if (c == '"') {
		QString r = readThing('"', true, false);
		// read next space
		socket_.getChar(0);
		return r;
	}
	return "";
}

QString SagittariusRepl::readThing(char delim, bool escape, bool wait)
{
	QString buf;
	// TODO make this configurable
	if (wait) socket_.waitForReadyRead(5000);
	for (;;) {
		char c;
		if (!socket_.getChar(&c)) break;
		if (escape && c == '\\') {
			if (!socket_.getChar(&c)) break;
			// we don't check that much but at lease \a \n \r \t
			switch (c) {
			case 'a': case 'n': case 'r': case 't':
				buf.append(c - 'a');
				break;
			default:
				buf.append(c);
				break;
			}
			continue;
		}
		if (c == delim) break;
		buf.append(c);
	}
	return buf;
}

// TODO handle states properly
void SagittariusRepl::observe(QProcess::ProcessState newState)
{
	qDebug() << "Process status changed: newState=" << newState;
	if (newState == QProcess::Starting) {
		qDebug() << "Starting remote REPL.";
	} else if (newState == QProcess::Running){
		// wait if it doesn't exist
		QFile mark(MARK_FILE);
		for (;;) {
			if (mark.exists()) break;
			sleep(5);
		}
		// connect to remote REPL
		socket_.connectToHost("localhost", config_->port());
		if (socket_.waitForConnected()) {
			QString tag = readTag(true);
			qDebug() << "Received: " << tag;
			// TODO handle other error
			if (tag == ":no-authenticate") {
				running_ = true;
				emit ready();
			}
		}
	} else {
		// something wrong or it's finished
		qDebug() << "Finished remote REPL. exit code=" << repl_.exitCode();
		socket_.close();
		flushProcess();
		emit finished();
	}
}

void SagittariusRepl::socketError(QAbstractSocket::SocketError socketError)
{
	qDebug() << "socket connection failed: " << socketError;
	error_ = true;
}

void SagittariusRepl::onFinished()
{
	running_ = false;
	if (error_) {
		error_ = false;
		emit error();
	}
}

void SagittariusRepl::flushProcess()
{
	QByteArray m = repl_.readAllStandardError();
	qDebug() << "stderr: " << QString(m);
	m = repl_.readAllStandardOutput();
	qDebug() << "stdout: " << QString(m);
}
