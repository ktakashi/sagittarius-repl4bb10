// Application with the Sheet project template
#include "SagittariusRepl.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include <QProcess>
#include <QDebug>
#include <QFileInfo>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>


using namespace bb::cascades;

#define REMOTE_PORT 12345

SagittariusRepl::SagittariusRepl(bb::cascades::Application *app)
: QObject(app)
{
	// create scene document from main.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	// create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();
	// set created root object as a scene
	app->setScene(root);

	qml->setContextProperty("repl", this);
}

SagittariusRepl::~SagittariusRepl()
{
	execute("(exit)");
}

QString SagittariusRepl::execute(const QString &expr)
{

	return "";
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

	QObject::connect(&repl_, SIGNAL(stateChanged(QProcess::ProcessState)),
			this, SLOT(observe(QProcess::ProcessState)));
	QObject::connect(&socket_, SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(socketError(QAbstractSocket::SocketError)));

	repl_.start("app/native/lib/sash -Dapp/native/modules -Lapp/native/scheme -d app/native/repl.scm");
	//repl_.start("/bin/sh", QStringList() << "app/native/invoke.sh");
	return true;
}

QString SagittariusRepl::readTag()
{
	QString buf;
	for (;;) {
		char c;
		if (!socket_.getChar(&c)) break;
		if (c == ' ') break;
		buf.append(c);
	}
	return buf;
}

// TODO handle states properly
void SagittariusRepl::observe(QProcess::ProcessState newState)
{
	qDebug() << "Process status changed: newState=" << newState;
	if (newState != QProcess::Running) {
		qDebug() << "Finished remote REPL. exit code=" << repl_.exitCode();
		QByteArray m = repl_.readAllStandardError();
		qDebug() << "stderr: " << QString(m);
		m = repl_.readAllStandardOutput();
		qDebug() << "stdout: " << QString(m);
	} else {
		// wait if it doesn't exist
		QFile mark(MARK_FILE);
		for (;;) {
			if (mark.exists()) break;
			sleep(5);
		}
		// connect to remote REPL
		socket_.connectToHost("localhost", REMOTE_PORT);
		if (socket_.waitForConnected()) {
			QString tag = readTag();
			qDebug() << "Received: " << tag;
		}
	}
}

void SagittariusRepl::socketError(QAbstractSocket::SocketError socketError)
{
	qDebug() << "socket connection failed: " << socketError;
}
