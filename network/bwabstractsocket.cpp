//
// C++ Implementation: BWAbstractSocket
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <bwabstractsocket.h>

/**
\brief Construtor
*/
BWAbstractSocket::BWAbstractSocket() {
}

/**
\brief Destrutor
*/
BWAbstractSocket::~BWAbstractSocket() {
}

/**
\brief Set the value of m_timeout
\param new_var the new value of m_timeout
*/
void BWAbstractSocket::SetTimeout(int new_var) {
    m_timeout = new_var;
}

/**
\brief Get the value of m_timeout
\return the value of m_timeout
*/
int BWAbstractSocket::GetTimeout() {
    return m_timeout;
}

/**
\brief Get the value of m_fd
\return the value of m_fd
*/
int BWAbstractSocket::GetFd() {
    return m_fd;
}

