//
// C++ Interface: BWSocket
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWABSTRACTSOCKET_H
#define BWABSTRACTSOCKET_H

#include <bwstring.h>
#include <bwerror.h>

/**
Classe abstrata para implementacao de Sockets.

	@author Samuel da Silva Feitosa <samuel@beanweb.com.br>
*/

class BWAbstractSocket {

    public:

        BWAbstractSocket();
        virtual ~BWAbstractSocket();

        virtual void Open() = 0;
        virtual void Close() = 0;

        virtual void Listen() = 0;
        BWAbstractSocket *Accept();
        virtual void Connect() = 0;

        virtual bool HasData(int timeout = -1) = 0;

        virtual int Send(const BWString &data) = 0;
        virtual int Send(const BWString &data, int size) = 0;
        virtual int Read(BWString *recvdata, int size) = 0;
        virtual BWString Read(int size) = 0;
        virtual int WaitData(BWString *recvdata, int size, int timeout = -1) = 0;
        virtual int ReadUntil(BWString *recvdata, const BWString &delim, int maxsize = 0) = 0;
        virtual BWString ReadUntil(const BWString &delim, int maxsize = 0) = 0;

        virtual bool IsOpen() = 0;

	int GetFd();

        void SetTimeout(int new_var);
        int GetTimeout();

    protected:
    	int m_fd;

    private:
        int m_timeout;
};

#endif // BWABSTRACTSOCKET_H
