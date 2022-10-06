//
// C++ Interface: bwnfinterface
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWNFINTERFACE_H
#define BWNFINTERFACE_H

extern "C" {
#include <libnfnetlink/libnfnetlink.h>
}

#include <bwstring.h>
#include <errno.h>

/**
Manipulacao da estrutura "nlfi_handle", a qual possibilita a obtencao dos nomes dos dispositivos de rede atraves de seu identificador numerico.

	@author Samuel Feitosa <samuel@beanweb.com.br>
*/
class BWNfInterface{
public:
    BWNfInterface();
    ~BWNfInterface();

    BWString GetName(int index);
    int GetFd();
    void FetchTable();
    bool HadEvent();

private:
    void init();

    struct nlif_handle *m_handler;
    int m_fd;
};

#endif
