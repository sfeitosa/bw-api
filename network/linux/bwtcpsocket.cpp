//
// C++ Implementation: BWTCPSocket
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <bwtcpsocket.h>

/**
\brief Construtor
\param host Host a conectar/receber conexao
\param port Porta a conectar/receber conexao
\param timeout Timeout da comunicacao
*/
BWTCPSocket::BWTCPSocket(const BWString &host, int port, int timeout) :
        BWSocket(timeout) {

    m_domain = AF_INET;
    m_type = SOCK_STREAM;
    m_proto = IPPROTO_IP;
    m_backlog = BW_BACKLOG;
    m_open = false;

    setPeerAddr(host, port);
}

/**
\brief Construtor
\param host Host a conectar/receber conexao
\param port Porta a conectar/receber conexao
\param timeout Timeout da comunicacao
\param fd Descritor do socket ja aberto
*/
BWTCPSocket::BWTCPSocket(const BWString &host, int port, int timeout, int fd) :
        BWSocket(timeout, fd) {

    m_domain = AF_INET;
    m_type = SOCK_STREAM;
    m_proto = IPPROTO_IP;
    m_backlog = BW_BACKLOG;
    m_open = false;

    setPeerAddr(host, port);
}

/**
\brief Destrutor
*/
BWTCPSocket::~BWTCPSocket() {
    if (IsOpen()) {
        Close();
    }
}

/**
\brief Prepara o processo de um "listener"
*/
void BWTCPSocket::Listen() {
    BWString oMsg;

    if (bind(GetFd(), BW_CSOCKADDR(&m_peer), sizeof(m_peer)) == -1) {
        oMsg = strerror(errno);
    
        throw BWError(SOCK_ERROR, oMsg);
    }

    if (listen(GetFd(), m_backlog) == -1) {
    	oMsg = strerror(errno);

	throw BWError(SOCK_ERROR, oMsg);
    }
}


/**
\brief Realiza a conexao de um Socket.
\return BWAcceptedServer
*/
void BWTCPSocket::Connect() {
    BWString oMsg;

    if (connect(GetFd(), BW_CSOCKADDR(&m_peer), sizeof(m_peer)) == -1) {
        oMsg = strerror(errno);
    
    	throw BWError(SOCK_ERROR, oMsg);
    }

    m_open = true;
}


/**
\brief Aceita uma conexao
\return BWAcceptedServer
*/
BWAbstractSocket *BWTCPSocket::Accept() {
    int new_fd;
    socklen_t len;
    BWAbstractSocket *sock;
    BWString oMsg;
    BWString ip;

    len = sizeof(m_peer);

    new_fd = accept(GetFd(), BW_CSOCKADDR(&m_peer), &len);

    if (new_fd == -1) {
        oMsg = strerror(errno);

        throw BWError(SOCK_ERROR, oMsg);
    }

    ip = inet_ntoa(m_peer.sin_addr);

    sock = new BWTCPSocket(ip, ntohs(m_peer.sin_port), GetTimeout(), new_fd);

    return sock;
}


/**
\brief Inicializa a estrutura sockaddr_in
\param host BWString contendo o IP ou host
\param port Porta TCP
*/
void BWTCPSocket::setPeerAddr(const BWString &host, int port) {
    BWString oMsg;
    BW_SOCKADDR *addr;

    if (host == "") {
        oMsg = "Host nao informado";
    
        throw BWError(SOCK_ERROR, oMsg);
    }

    SetHost(host);
    SetCommPort(port);

    addr = resolve(host);

    memcpy(&m_peer, addr, sizeof(BW_SOCK_IN));

    m_peer.sin_family = m_domain;
    m_peer.sin_port = htons(port);
}

/**
\brief Set the value of m_host
\param new_var the new value of m_host
*/
void BWTCPSocket::SetHost(BWString new_var) {
    m_host = new_var;
}

/**
\brief Get the value of m_host
\return the value of m_host
 */
BWString BWTCPSocket::GetHost() {
    return m_host;
}

/**
\brief Set the value of m_port
\param new_var the new value of m_port
*/
void BWTCPSocket::SetCommPort(int new_var) {
    m_port = new_var;
}

/**
\brief Get the value of m_port
\return the value of m_port
*/
int BWTCPSocket::GetPort() {
    return m_port;
}
