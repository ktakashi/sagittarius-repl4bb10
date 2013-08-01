// Application with the Sheet project template
#ifndef SagittariusRepl_HPP_
#define SagittariusRepl_HPP_

#include <QObject>
#include <QProcess>
#include <QTcpSocket>

namespace bb { namespace cascades { class Application; }}

class ReplConfig;
/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class SagittariusRepl : public QObject
{
	Q_OBJECT
public:
	SagittariusRepl(bb::cascades::Application *app);
	virtual ~SagittariusRepl();

	bool initRemoteREPL();

	Q_INVOKABLE QString execute(const QString &expr);
	Q_INVOKABLE void    restart();
public slots:
	void observe(QProcess::ProcessState newState);
	void socketError(QAbstractSocket::SocketError socketError);
	void onFinished();

signals:
	void ready();
	void error();
	void finished();

private:
	QString readTag(bool wait = false);
	int     readValuesCount();
	QString readDatum();
	QString readThing(char delim, bool escape = false, bool wait = false);

	void flushProcess();

	// we are using remote-repl to make things easier...
	QProcess repl_;
	QTcpSocket socket_;
	bool running_;
	bool error_;
	ReplConfig *config_;
};

#endif /* SagittariusRepl_HPP_ */
