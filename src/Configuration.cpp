/*
 * Configuration.cpp
 *
 *  Created on: 1 aug. 2013
 *      Author: t.kato
 */


#include "Configuration.hpp"

#define REMOTE_PORT 12345

ReplConfig::ReplConfig()
: port_(REMOTE_PORT)
{}

int ReplConfig::port() const
{
	return port_;
}

void ReplConfig::setPort(int port)
{
	port_ = port;
	emit portChanged(port_);
}
