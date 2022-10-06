//
// C++ Implementation: bwnfinterface
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bwnfinterface.h"

BWNfInterface::BWNfInterface()
{
    m_handler = NULL;
    m_fd = -1;
    init();
}


BWNfInterface::~BWNfInterface()
{
    if (m_handler) {
        nlif_close(m_handler);
    }
}


/*!
    \fn BWNfInterface::init()
 */
void BWNfInterface::init()
{
    m_handler = nlif_open();

    if (!m_handler) {
        throw BWError(BASE_ERROR,
          "Impossivel abrir manipulador de interfaces (nlif_open)");
    }

    m_fd = nlif_fd(m_handler);

    nlif_query(m_handler);
}


/*!
    \fn BWNfInterface::FetchTable()
 */
void BWNfInterface::FetchTable()
{
    if (HadEvent()) {
        nlif_catch(m_handler);
    }
}


/*!
    \fn BWNfInterface::GetFd()
 */
int BWNfInterface::GetFd()
{
    return m_fd;
}


/*!
    \fn BWNfInterface::GetName()
 */
BWString BWNfInterface::GetName(int index)
{
    BWString ifname;
    char name[IFNAMSIZ];

    if (nlif_index2name(m_handler, index, name) == -1) {
        return "";
    }

    ifname = name;

    return ifname;
}


/*!
    \fn BWNfInterface::HadEvent()
 */
bool BWNfInterface::HadEvent()
{
    fd_set sFds;
    struct timeval tv;
    int nRet;
    BWString oMsg;

    FD_ZERO(&sFds);
    FD_SET(m_fd, &sFds);

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    nRet = select(m_fd + 1, &sFds, NULL, NULL, &tv);

    if (nRet == -1) {
        oMsg = strerror(errno);

        throw BWError(BASE_ERROR, oMsg);
    }

    if (nRet == 1) {
        return true;
    }

    return false;
}

