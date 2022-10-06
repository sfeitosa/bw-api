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
#include <bwclient.h>

/**
\brief Construtor
\param host Endereco IP do servidor a conectar.
\param port Porta TCP do servidor a conectar.
\param timeout Timeout da conexao TCP.
*/
BWClient::BWClient(const BWString &host, int port, int timeout) :
        BWTCPSocket(host, port, timeout) {

	m_open = false;
}

/**
\brief Destrutor
*/
BWClient::~BWClient() {

}

