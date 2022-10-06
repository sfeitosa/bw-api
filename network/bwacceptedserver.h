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
#ifndef BWACCEPTEDSERVER_H
#define BWACCEPTEDSERVER_H

#include <bwtcpsocket.h>
#include <bwerror.h>

/**
Classe que implementa as funcionalidades de Sockets apos o recebimento de uma conexao.
	@author Samuel da Silva Feitosa <samuel@beanweb.com.br>
*/

/**
\file BWAcceptedServer.h
\brief Classe que representa Sockets conectados
*/

class BWAcceptedServer: private BWTCPSocket {

    public:

        BWAcceptedServer(const BWString &host, int port,
                         int timeout, int fd);
        virtual ~BWAcceptedServer();
        
        BWString GetClientAddr();
        int GetClientPort();

        BWTCPSocket::Send;
        BWTCPSocket::HasData;
        BWTCPSocket::Read;
        BWTCPSocket::ReadUntil;
        BWTCPSocket::WaitData;
        BWTCPSocket::Close;
        BWTCPSocket::GetFd;

};

#endif // BWACCEPTEDSERVER_H
