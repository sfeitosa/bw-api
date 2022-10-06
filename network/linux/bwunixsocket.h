//
// C++ Interface: BWUnixSocket
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWUNIXSOCKET_H
#define BWUNIXSOCKET_H

#include <bwsocket.h>

#include <sys/un.h>

/**
Classe que implementa Sockets Unix.

	@author Samuel da Silva Feitosa <samuel@beanweb.com.br>
*/

/**
\typedef Define a estrutura sockaddr_un
*/

typedef struct sockaddr_un BW_SOCK_UN;

class BWUnixSocket : public BWSocket {

    public:
        BWUnixSocket(const BWString &path, int timeout);
        BWUnixSocket(const BWString &path, int timeout, int fd);
        ~BWUnixSocket();

	void Listen();
	void Connect();

        BWAbstractSocket *Accept();

	BWString GetPath();

    private:
        void setPeerAddr(const BWString &path);

        BW_SOCK_UN m_peer;
        int m_backlog;
	BWString m_path;
};

#endif // BWTCPSOCKET_H

