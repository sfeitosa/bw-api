//
// C++ Implementation: bwudppacket
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bwudppacket.h"

BWUdpPacket::BWUdpPacket()
 : BWNetPacket()
{
    m_packet = NULL;
    m_udphdr = NULL;
}

/*!
    \fn BWUdpPacket::BWUdpPacket(char *packet)
 */
 BWUdpPacket::BWUdpPacket(unsigned char *packet)
{
    m_packet = NULL;
    m_udphdr = NULL;

    Decode(packet);
}


BWUdpPacket::~BWUdpPacket()
{
}


/*!
    \fn BWUdpPacket::Encode()
 */
void BWUdpPacket::Encode()
{
    /// @todo implement me
}


/*!
    \fn BWUdpPacket::Decode(char *packet)
 */
void BWUdpPacket::Decode(unsigned char *packet)
{
    m_packet = packet;
    m_udphdr = (struct udphdr *) packet;
}


/*!
    \fn BWUdpPacket::GetData()
 */
unsigned char * BWUdpPacket::GetData()
{
    unsigned char *ptr = NULL;

    if (!m_packet || !m_udphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    ptr = m_packet + 2;

    return ptr;
}


/*!
    \fn BWUdpPacket::GetSourcePort()
 */
unsigned int BWUdpPacket::GetSourcePort()
{
    if (!m_udphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_udphdr->source);
}


/*!
    \fn BWUdpPacket::GetDestPort()
 */
unsigned int BWUdpPacket::GetDestPort()
{
    if (!m_udphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_udphdr->dest);
}


/*!
    \fn BWUdpPacket::GetLength()
 */
unsigned int BWUdpPacket::GetLength()
{
    if (!m_udphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_udphdr->len);
}


/*!
    \fn BWUdpPacket::GetChecksum()
 */
unsigned int BWUdpPacket::GetChecksum()
{
    if (!m_udphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_udphdr->check);
}
