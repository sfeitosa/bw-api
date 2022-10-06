//
// C++ Interface: BWTCPSocket
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWTCPSOCKET_H
#define BWTCPSOCKET_H

#include <bwsocket.h>

/**
Classe que implementa Sockets atraves do protocolo TCP/IP.

	@author Samuel da Silva Feitosa <samuel@beanweb.com.br>
*/

#define BW_BACKLOG 5
#define BW_MAX_PACKET_SIZE 1024

/**
\typedef Define a estrutura sockaddr_in.
*/

typedef struct sockaddr_in BW_SOCK_IN;

/**
\typedef Define a estrutura in_addr.
*/

typedef struct in_addr BW_ADDR_IN;

class BWTCPSocket : public BWSocket {

    public:
        BWTCPSocket(const BWString &host, int port, int timeout);
        BWTCPSocket(const BWString &host, int port, int timeout, int fd);
        ~BWTCPSocket();

        void Listen();
        void Connect();

        BWAbstractSocket *Accept();

        void SetHost(BWString new_var);
        void SetCommPort(int new_var);

        BWString GetHost();
        int GetPort();


    private:
        void setPeerAddr(const BWString &ip, int port);

        BW_SOCK_IN m_peer;
        int m_backlog;
        BWString m_host;
        int m_port;
};

#endif // BWTCPSOCKET_H

