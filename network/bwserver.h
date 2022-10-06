//
// C++ Interface: BWServer
//
// Description: 
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWSERVER_H
#define BWSERVER_H

#include <bwtcpsocket.h>
#include <bwacceptedserver.h>
#include <bwerror.h>

/**
Classe que implementa as funcionalidades de um servidor.

	@author Samuel da Silva Feitosa <samuel@beanweb.com.br>
*/

/**
\file bwserver.h
\brief Classe que implementa um servidor TCP.
*/

class BWServer : private BWTCPSocket {

    public:
        BWServer(const BWString &host, int port, int timeout);
        ~BWServer();

	// Metodos herdados da classe anscestral
        BWAcceptedServer *Accept(void);
};

#endif // BWSERVER_H
