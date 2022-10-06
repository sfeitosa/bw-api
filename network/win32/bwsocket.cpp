//
// C++ Implementation: BWSocket
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <bwsocket.h>

/**
\brief Construtor
\param host Host a conectar/receber conexao
\param port Porta a conectar/receber conexao
\param timeout Timeout da conexao
*/
BWSocket::BWSocket(int timeout):
	BWAbstractSocket()
{
    SetTimeout(timeout);

    init();
}

/**
\brief Construtor
\param host Host a conectar/receber conexao
\param port Porta a conectar/receber conexao
\param timeout Timeout da conexao
\param fd Descritor do socket conectado
\note Construtor de escopo protegido para inicializar o objeto 
BWAcceptedServer
*/
BWSocket::BWSocket(int timeout, int fd) {
    SetTimeout(timeout);
    m_fd = fd;

    init();
}


/**
\brief Destrutor
*/
BWSocket::~BWSocket() {
    WSACleanup();
}

/**
\brief Abre uma conexao de Socket.
*/
void BWSocket::Open() {
    int opt = 1;
    BWString oMsg;

    m_fd = socket(m_domain, m_type, m_proto);

    setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));

    if (m_fd < 0) {
        oMsg = strerror(errno);
    	
	throw BWError(SOCK_ERROR, oMsg);
    }
}


/**
\brief Fecha uma conexao de Socket.
*/
void BWSocket::Close() {
    closesocket(m_fd);

    m_open = false;
}


/**
\brief Verifica se ha dados a serem lidos no socket.
\param timeout Tempo maximo para aguardar pelo recebimento de dados. 
Caso seja informado o valor -1, aguarda o timeout padrao da classe.
Caso seja informado o valor -2, aguarda ate que hajam dados disponiveis.
\return true ou false
*/
bool BWSocket::HasData(int timeout) {
    fd_set sFds;
    struct timeval tv;
    int nRet;
    BWString oMsg;
	unsigned int fd = m_fd;

    FD_ZERO(&sFds);
    FD_SET(fd, &sFds);

    if (timeout == -1) {
        tv.tv_sec = GetTimeout();
    }
    else {
        tv.tv_sec = timeout;
    }

    tv.tv_usec = 0;

    if (timeout == -2) {
        nRet = select(m_fd + 1, &sFds, NULL, NULL, NULL);
    }
    else {
        nRet = select(m_fd + 1, &sFds, NULL, NULL, &tv);
    }

    if (nRet == -1) {
        oMsg = strerror(errno);

	throw BWError(SOCK_ERROR, oMsg);
    }

    if (nRet == 1) {
        return true;
    }

    return false;
}

void BWSocket::init() {
    WSAData wsaData;
	int err = 0;

    err = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (err != 0) {
        // lancar excessao
    }

}

/**
\brief Resolve um host em um endereco IP
\param host IP ou nome da maquina
\return BWString contendo o IP da maquina
*/
BW_SOCKADDR *BWSocket::resolve(const BWString &host) {
    struct addrinfo *result;
    int error;
    BWString oMsg;

    error = getaddrinfo(host.c_str(), NULL, NULL, &result);

    if (error != 0) {
        oMsg = gai_strerror(errno);
    	
	throw BWError(SOCK_ERROR, oMsg);
    }
	
    if (result == NULL) {
        oMsg = "Impossivel resolver HOST para um IP";

	throw BWError(SOCK_ERROR, oMsg);
    }

    return result->ai_addr;
}

bool BWSocket::IsOpen(void) {
	return m_open;
}

/**
\brief Envia dados pelo Socket.
\param  data Dados a serem enviados pelo socket.
\return Numero de bytes enviados
*/
int BWSocket::Send(const BWString &data) {
    return Send(data, 0);
}


/**
\brief Envia dados pelo Socket de acordo com o tamanho definido em "size".
\param  data BWString contendo os dados a serem enviados.
\param  size Quantidade de bytes a enviar. Caso receba o valor zero, envia todo
o conteudo do objeto BWString.
\return Numero de bytes enviados
*/
int BWSocket::Send(const BWString &data, int size) {
    int nBytes;
    int nSize;
    BWString oMsg;

    if (size > 0) {
        nSize = size;
    }
    else {
        nSize = data.size();
    }

    nBytes = send(GetFd(), data.c_str(), nSize, 0);

    if (nBytes <= 0) {
        oMsg = strerror(errno);
    	
	throw BWError(SOCK_ERROR, oMsg);

        m_open = false;
    }

    return nBytes;
}


/**
\brief Espera por dados em um Socket.
\param  recvdata Ponteiro para armazenar dados de recebidos atraves do Socket.
\param  timeout Tempo maximo para aguardar pelo recebimento de dados. Caso seja
informado o valor 0, espera indefinidamente. Caso seja informado o valor -1,
espera o timeout definido na classe.
\return Retorna o numero de bytes lidos.
*/
int BWSocket::WaitData(BWString *recvdata, int size, int timeout) {
    int nBytes = 0;

    if (HasData(timeout)) {
        nBytes = Read(recvdata, size);
    }

    return nBytes;
}

BWString BWSocket::WaitData(int size, int timeout) {
    BWString tmp;

    WaitData(&tmp, size, timeout);

    return tmp;
}

int BWSocket::Read(BWString *recvdata, int size) {
    int nRead;
    int nBytes = 0;
    char readBytes[2];

    do {
        memset(readBytes, 0, 2);

	nRead = recv(GetFd(), readBytes, 1, 0);

        if (nRead > 0) {
	    nBytes += nRead;
            recvdata->append(readBytes);
	}

        if (nRead == 0) {
            m_open = false;
        }

    } while (nRead > 0 && nBytes < size);

    return nBytes;
}

BWString BWSocket::Read(int size) {
	BWString tmp;

	Read(&tmp, size);

	return tmp;
}


int BWSocket::ReadUntil(BWString *recvdata, const BWString &delim, 
                           int maxsize) {
    int nRead;
    int nBytes = 0;
    char readBytes[2];
    BWString oMsg;

    do {
        memset(readBytes, 0, 2);

        nRead = recv(GetFd(), readBytes, 1, 0);

        if (nRead > 0) {
	    nBytes += nRead;
	    recvdata->append(readBytes);
	}
	else if (nRead == 0) {
            m_open = false;
	    return nBytes;
	}
	else {
	    oMsg = strerror(errno);

	    throw BWError(SOCK_ERROR, oMsg);
	}

	if (recvdata->size() >= delim.size() &&
	    recvdata->compare(recvdata->size() - delim.size(), 
	    delim.size(), delim) == 0) {
	    break;
	}

    } while (nBytes < maxsize || maxsize == 0);

    return nBytes;
}

BWString BWSocket::ReadUntil(const BWString &delim, int maxsize) {
    BWString tmp;

    ReadUntil(&tmp, delim, maxsize);

    return tmp;
}

