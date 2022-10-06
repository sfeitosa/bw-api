//
// C++ Interface: bwippacket
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWIPPACKET_H
#define BWIPPACKET_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <bwnetpacket.h>

/**
Classe responsavel por encapsular um pacote de rede IP.

	@author Samuel Feitosa <samuel@beanweb.com.br>
*/
class BWIpPacket : public BWNetPacket
{
public:
    BWIpPacket();
    BWIpPacket(unsigned char *, int);

    ~BWIpPacket();
    void Encode();
    void Decode(unsigned char *);
    unsigned char *GetData();
    unsigned short GetId();
    unsigned short GetLength();
    char GetTOS();
    unsigned short GetChecksum();
    unsigned int GetProtocol();
    char GetTTL();
    BWString GetDestIp();
    BWString GetSourceIp();
    BWString GetProtoName();
    void SetPacketLen(int len);

private:
    int getPacketSize();

    unsigned char *m_packet;
    struct iphdr *m_iphdr;
    int m_packetlen;

};

#endif
