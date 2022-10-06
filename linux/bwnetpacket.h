//
// C++ Interface: bwnetpacket
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWNETPACKET_H
#define BWNETPACKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bwstring.h>
#include <bwerror.h>

/**
Classe responsavel por encapsular um pacote ("frame") de rede.

	@author Samuel Feitosa <samuel@beanweb.com.br>
*/
class BWNetPacket {
public:
    BWNetPacket();

    virtual ~BWNetPacket();
    virtual void Encode() = 0;
    virtual void Decode(unsigned char *) = 0;

protected:
    void checksum();

};

#endif
