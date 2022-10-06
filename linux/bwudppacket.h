//
// C++ Interface: bwudppacket
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWUDPPACKET_H
#define BWUDPPACKET_H

#include <bwnetpacket.h>
#include <netinet/udp.h>

/**
Classe responsavel por encapsular um pacote UDP.

	@author Samuel Feitosa <samuel@beanweb.com.br>
*/
class BWUdpPacket : public BWNetPacket
{
public:
    BWUdpPacket();

    ~BWUdpPacket();
     BWUdpPacket(unsigned char *packet);
    void Decode(unsigned char *packet);
    void Encode();
    unsigned char *GetData();
    unsigned int GetSourcePort();
    unsigned int GetDestPort();
    unsigned int GetChecksum();
    unsigned int GetLength();

private:
    unsigned char *m_packet;
    struct udphdr *m_udphdr;
};

#endif
