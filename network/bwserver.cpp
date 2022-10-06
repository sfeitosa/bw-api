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
#include <bwserver.h>

/**
\brief  Construtor.
\param  host IP onde o servidor devera aguardar por conexoes.
\param  port Porta na qual o servidor devera aguardar por conexoes.
\param  timeout Tempo padrao para estabelecimento de conexao.
*/
BWServer::BWServer(const BWString &host, int port, int timeout) :
        BWTCPSocket(host, port, timeout) {

    try {
        Open();
        Listen();
    }
    catch (BWError &e) {
        Close();
        throw;
    }
}


/**
\brief Destrutor
*/
BWServer::~BWServer() {

}

BWAcceptedServer *BWServer::Accept() {
	BWTCPSocket *sock;
	BWAcceptedServer *new_sock;

	sock = reinterpret_cast<BWTCPSocket *>(BWTCPSocket::Accept());

	new_sock = new BWAcceptedServer(sock->GetHost(), sock->GetPort(), 
					sock->GetTimeout(), sock->GetFd());

	delete sock;

	return new_sock;
}

