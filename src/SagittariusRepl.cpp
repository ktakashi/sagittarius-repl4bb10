// Application with the Sheet project template
#include "SagittariusRepl.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include <QProcess>
#include <QDebug>
#include <QFileInfo>
#include <sys/types.h>
#include <signal.h>


using namespace bb::cascades;

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
}

SagittariusRepl::~SagittariusRepl()
{

}

static bool sanity(const QString &name, bool checkExec = false)
{
	// sanity check
	QFileInfo info(name);
	if (!info.exists()) return false;
	if (!info.isReadable()) return false;
	if (checkExec && !info.isExecutable()) return false;
	return true;
}

bool SagittariusRepl::initRemoteREPL()
{
	if (!sanity("app/native/invoke.sh")) return false;
	if (!sanity("app/native/lib/sash", true)) return false;
	if (!sanity("app/native/lib/libgcmt-dll.so")) return false;
	if (!sanity("app/native/lib/libsagittarius.so")) return false;

	//repl_.start("app/native/sash -Dapp/native/modules -Lapp/native/scheme -d app/native/repl.scm");
	repl_.start("/bin/sh", QStringList() << "app/native/invoke.sh");

	QStringList envs = QProcess::systemEnvironment();
	envs.sort();

	for (QStringList::iterator itr = envs.begin();
		itr != envs.end(); itr++) {
		qDebug() << *itr;
	}
	QObject::connect(&repl_, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(observe(QProcess::ProcessState)));
	return true;
}

void SagittariusRepl::observe(QProcess::ProcessState newState)
{
	qDebug() << "Process status changed: newState=" << newState;
	if (newState != QProcess::Running) {
		QProcess::ProcessError e = repl_.error();
		qDebug() << "Remote REPL failed. error code="  << e << ", exit code=" << repl_.exitCode();
		QByteArray m = repl_.readAllStandardError();
		qDebug() << "stderr: " << QString(m);
		m = repl_.readAllStandardOutput();
		qDebug() << "stdout: " << QString(m);
	}
}
