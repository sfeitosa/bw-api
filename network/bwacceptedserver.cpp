//
// C++ Interface: BWAcceptedServer
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <bwacceptedserver.h>

/**
\brief Construtor
*/
BWAcceptedServer::BWAcceptedServer(const BWString &host, int port,
                                   int timeout, int fd) :
        BWTCPSocket(host, port, timeout, fd) {
	m_open = true;
}


/**
\brief Destrutor
*/
BWAcceptedServer::~BWAcceptedServer() {

}

BWString BWAcceptedServer::GetClientAddr() {
    return GetHost();
}

int BWAcceptedServer::GetClientPort() {
    return GetPort();
}

