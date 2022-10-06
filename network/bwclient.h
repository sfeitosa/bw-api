//
// C++ Interface: BWClient
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWCLIENT_H
#define BWCLIENT_H

#include <bwtcpsocket.h>
#include <bwerror.h>

/**
Classe que implementa Sockets Client.

	@author Samuel da Silva Feitosa <samuel@beanweb.com.br>
*/

class BWClient: private BWTCPSocket {

    public:
        BWClient(const BWString &host, int port, int timeout);
        ~BWClient();

        // Metodos herdados da classe anscestral
	BWTCPSocket::Open;
        BWTCPSocket::Connect;
        BWTCPSocket::Send;
        BWTCPSocket::Read;
        BWTCPSocket::ReadUntil;
        BWTCPSocket::WaitData;
        BWTCPSocket::HasData;
        BWTCPSocket::Close;
        BWTCPSocket::IsOpen;

};

#endif // BWCLIENT_H
