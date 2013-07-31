// Application with the Sheet project template
#ifndef SagittariusRepl_HPP_
#define SagittariusRepl_HPP_

#include <QObject>
#include <QProcess>
#include <QTcpSocket>

namespace bb { namespace cascades { class Application; }}

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

	QString execute(const QString &expr);
public slots:
	void observe(QProcess::ProcessState newState);
	void socketError(QAbstractSocket::SocketError socketError);
private:
	QString readTag();


	// we are using remote-repl to make things easier...
	QProcess repl_;
	QTcpSocket socket_;
};

#endif /* SagittariusRepl_HPP_ */
