//
// C++ Implementation: BWUnixSocket
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <bwunixsocket.h>

/**
\brief Construtor
\param path Caminho para o socket
\param timeout Timeout da comunicacao
*/
BWUnixSocket::BWUnixSocket(const BWString &path, int timeout) :
        BWSocket(timeout) {

    m_domain = AF_UNIX;
    m_type = SOCK_STREAM;
    m_proto = 0;
    m_backlog = BW_BACKLOG;
    m_open = false;

    setPeerAddr(path);
}

/**
\brief Construtor
\param path Caminho para o socket
\param timeout Timeout da comunicacao
\param fd Descritor do socket ja aberto
*/
BWUnixSocket::BWUnixSocket(const BWString &path, int timeout, int fd) :
        BWSocket(timeout, fd) {

    m_domain = AF_UNIX;
    m_type = SOCK_STREAM;
    m_proto = 0;
    m_backlog = BW_BACKLOG;
    m_open = false;

    setPeerAddr(path);
}

/**
\brief Destrutor
*/
BWUnixSocket::~BWUnixSocket() {
    if (IsOpen()) {
        Close();
    }
}

/**
\brief Prepara o processo de um "listener"
*/
void BWUnixSocket::Listen() {
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
*/
void BWUnixSocket::Connect() {
    BWString oMsg;

    if (connect(GetFd(), BW_CSOCKADDR(&m_peer), sizeof(m_peer)) == -1) {
        oMsg = strerror(errno);
    
    	throw BWError(SOCK_ERROR, oMsg);
    }

    m_open = true;
}


/**
\brief Aceita uma conexao
\return BWAbstractSocket
*/
BWAbstractSocket *BWUnixSocket::Accept() {
    int new_fd;
    socklen_t len;
    BWAbstractSocket *sock;
    BWString oMsg;

    len = sizeof(m_peer);

    new_fd = accept(GetFd(), BW_CSOCKADDR(&m_peer), &len);

    if (new_fd == -1) {
        oMsg = strerror(errno);

        throw BWError(SOCK_ERROR, oMsg);
    }

    sock = new BWUnixSocket(GetPath(), GetTimeout(), new_fd);

    return sock;
}


/**
\brief Inicializa a estrutura sockaddr_un
\param path BWString contendo o caminho para o socket
*/
void BWUnixSocket::setPeerAddr(const BWString &path) {
    BWString oMsg;

    if (path == "" && path.length() > 108) {
        oMsg = "Caminho para o socket informado incorretamente";
    
        throw BWError(SOCK_ERROR, oMsg);
    }

    m_path = path;

    m_peer.sun_family = m_domain;
    strcpy(m_peer.sun_path, path.c_str());
}

/**
\brief Retorna o caminho referente ao socket
*/
BWString BWUnixSocket::GetPath() {
    return m_path;
}
