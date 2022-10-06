//
// C++ Interface: BWSocket
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWSOCKET_H
#define BWSOCKET_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#else
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

#include <bwabstractsocket.h>

/**
Classe base para implementacao de Sockets.

	@author Samuel da Silva Feitosa <samuel@beanweb.com.br>
*/

#define BW_BACKLOG 5

/**
\typedef Define a estrutura sockaddr.
*/
typedef struct sockaddr BW_SOCKADDR;

/**
\brief Faz um cast para sockaddr
*/
inline BW_SOCKADDR *BW_CSOCKADDR(void *addr) {
    return reinterpret_cast<BW_SOCKADDR *>(addr);
}

class BWSocket: public BWAbstractSocket {

    public:

        BWSocket(int timeout);
        BWSocket(int timeout, int fd);
        ~BWSocket();

        void Open();
        void Close();
        bool HasData(int timeout = -1);

        virtual void Listen() = 0;
        virtual void Connect() = 0;

        bool IsOpen();

        int Send(const BWString & data);
        int Send(const BWString & data, int size);
        int Read(BWString *recvdata, int size);
        BWString Read(int size);
        int WaitData(BWString *recvdata, int size, int timeout = -1);
        BWString WaitData(int size, int timeout = -1);
        int ReadUntil(BWString *recvdata, const BWString &delim,
                      int maxsize = 0);
        BWString ReadUntil(const BWString &delim, int maxsize = 0);

    protected:

        BW_SOCKADDR *resolve(const BWString &host);
        void init();

        int m_proto;
        int m_domain;
        int m_type;
        bool m_open;
};

#endif // BWSOCKET_H
