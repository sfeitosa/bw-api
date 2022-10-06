//
// C++ Interface: bwtcppacket
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWTCPPACKET_H
#define BWTCPPACKET_H

#include <bwnetpacket.h>
#include <netinet/tcp.h>

/**
Classe responsavel por encapsular pacotes de rede TCP.

	@author Samuel Feitosa <samuel@beanweb.com.br>
*/
class BWTcpPacket : public BWNetPacket
{
public:
    BWTcpPacket();
    BWTcpPacket(unsigned char *);
    ~BWTcpPacket();
    void Encode();
    void Decode(unsigned char *);
    unsigned char *GetData();
    unsigned short GetSourcePort();
    unsigned short GetDestPort();
    unsigned int GetAckNumber();
    unsigned int GetSeqNumber();
    unsigned short GetUrgPointer();
    unsigned short GetChecksum();
    unsigned short GetWindow();
    unsigned short GetOffset();

private:
    unsigned char *m_packet;
    struct tcphdr *m_tcphdr;
};

#endif
