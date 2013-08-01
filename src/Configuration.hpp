/*
 * Configuration.hpp
 *
 *  Created on: 1 aug. 2013
 *      Author: t.kato
 */

#ifndef CONFIGURATION_HPP_
#define CONFIGURATION_HPP_

#include <QObject>
#include <QString>

class ReplConfig : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
public:
	ReplConfig();
	~ReplConfig() {}

	int port() const;
	void setPort(int port);
signals:
	void portChanged(int);

private:
	int port_;
};


#endif /* CONFIGURATION_HPP_ */
