//
// C++ Implementation: bwippacket
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bwippacket.h"

/*!
    \fn BWIpPacket::BWIpPacket()
 */
BWIpPacket::BWIpPacket()
{
    m_packet = NULL;
    m_iphdr = NULL;
    m_packetlen = 0;
}


BWIpPacket::BWIpPacket(unsigned char *packet, int len) {
    m_packet = NULL;
    m_iphdr = NULL;
    m_packetlen = len;

    Decode(packet);
}


BWIpPacket::~BWIpPacket()
{
}

/*!
    \fn BWIpPacket::Encode()
 */
void BWIpPacket::Encode()
{
    /// @todo implement me
}


/*!
    \fn BWIpPacket::Decode(char *)
 */
void BWIpPacket::Decode(unsigned char *packet)
{
    if (!packet) {
        throw BWError(STRUCTURAL_ERROR, "Pacote IP mal formado.");
    }

    m_packet = packet;
    m_iphdr = (struct iphdr *) packet;
}


/*!
    \fn BWIpPacket::GetData()
 */
unsigned char *BWIpPacket::GetData()
{
    unsigned char *ptr;

    if (!m_packet || !m_iphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    if (m_packetlen < getPacketSize()) {
        throw BWError(BASE_ERROR, "Tamanho do pacote de protocolo invalido");
    }

    ptr = m_packet + 4 * m_iphdr->ihl;

    return ptr;
}


/*!
    \fn BWIpPacket::GetLength()
 */
unsigned short BWIpPacket::GetLength()
{
    if (!m_iphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_iphdr->tot_len);
}


/*!
    \fn BWIpPacket::GetId()
 */
unsigned short BWIpPacket::GetId()
{
    if (!m_iphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_iphdr->id);
}


/*!
    \fn BWIpPacket::GetTOS()
 */
char BWIpPacket::GetTOS()
{
    if (!m_iphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return m_iphdr->tos;
}


/*!
    \fn BWIpPacket::GetTTL()
 */
char BWIpPacket::GetTTL()
{
    if (!m_iphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return m_iphdr->ttl;
}


/*!
    \fn BWIpPacket::GetProtocol()
 */
unsigned int BWIpPacket::GetProtocol()
{
    if (!m_iphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return m_iphdr->protocol;
}


/*!
    \fn BWIpPacket::GetChecksum()
 */
unsigned short BWIpPacket::GetChecksum()
{
    if (!m_iphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_iphdr->check);
}


/*!
    \fn BWIpPacket::GetSourceIp()
 */
BWString BWIpPacket::GetSourceIp()
{
    struct in_addr addr;

    if (!m_iphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    addr.s_addr = m_iphdr->saddr;

    return inet_ntoa(addr);
}


/*!
    \fn BWIpPacket::GetDestIp()
 */
BWString BWIpPacket::GetDestIp()
{
    struct in_addr addr;

    if (!m_iphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    addr.s_addr = m_iphdr->daddr;

    return inet_ntoa(addr);
}


BWString BWIpPacket::GetProtoName() 
{
    unsigned int proto;

    proto = GetProtocol();

    switch (proto) {
        case IPPROTO_IP: return "IP";
        case IPPROTO_ICMP: return "ICMP";
        case IPPROTO_IGMP: return "IGMP";
        case IPPROTO_IPIP: return "IPIP";
        case IPPROTO_TCP: return "TCP";
        case IPPROTO_EGP: return "EGP";
        case IPPROTO_PUP: return "PUP";
        case IPPROTO_UDP: return "UDP";
        case IPPROTO_IDP: return "IDP";
        case IPPROTO_RSVP: return "RSVP";
        case IPPROTO_GRE: return "GRE";
        case IPPROTO_IPV6: return "IPV6";
        case IPPROTO_ESP: return "ESP";
        case IPPROTO_AH: return "AH";
        case IPPROTO_PIM: return "PIM";
        case IPPROTO_COMP: return "COMP";
        case IPPROTO_SCTP: return "SCTP";
        case IPPROTO_RAW: return "RAW";
        default: return "UNKNOWN";
    }
}

int BWIpPacket::getPacketSize() 
{
    unsigned int proto = GetProtocol();
    int size = sizeof(struct iphdr);

    switch (proto) {
       case IPPROTO_TCP: {
           size += sizeof(struct tcphdr);
           break;
       }
       case IPPROTO_UDP: {
           size += sizeof(struct udphdr);
           break;
       }
       default: {
          size += sizeof(struct tcphdr);
          break;
       }
    }

    return size;
}

void BWIpPacket::SetPacketLen(int len) 
{
    m_packetlen = len;
}
