//
// C++ Implementation: bwtcppacket
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bwtcppacket.h"

BWTcpPacket::BWTcpPacket()
{
    m_packet = NULL;
    m_tcphdr = NULL;
}


BWTcpPacket::BWTcpPacket(unsigned char *packet)
{
    m_packet = NULL;
    m_tcphdr = NULL;

    Decode(packet);
}


BWTcpPacket::~BWTcpPacket()
{
}


/*!
    \fn BWTcpPacket::Encode()
 */
void BWTcpPacket::Encode()
{
    /// @todo implement me
}


/*!
    \fn BWTcpPacket::Decode(char *)
 */
void BWTcpPacket::Decode(unsigned char *packet)
{
    m_packet = packet;
    m_tcphdr = (struct tcphdr *) packet;
}


/*!
    \fn BWTcpPacket::GetData()
 */
unsigned char * BWTcpPacket::GetData()
{
    unsigned char *ptr;

    if (!m_packet || !m_tcphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    ptr = m_packet + 4 * m_tcphdr->doff;

    return ptr;
}


/*!
    \fn BWTcpPacket::GetSourcePort()
 */
unsigned short  BWTcpPacket::GetSourcePort()
{
    if (!m_tcphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_tcphdr->source);
}


/*!
    \fn BWTcpPacket::GetDestPort()
 */
unsigned short BWTcpPacket::GetDestPort()
{
    if (!m_tcphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_tcphdr->dest);
}


/*!
    \fn BWTcpPacket::GetSeqNumber()
 */
unsigned int BWTcpPacket::GetSeqNumber()
{
    if (!m_tcphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohl(m_tcphdr->seq);
}


/*!
    \fn BWTcpPacket::GetAckNumber()
 */
unsigned int BWTcpPacket::GetAckNumber()
{
    if (!m_tcphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohl(m_tcphdr->ack_seq);
}


/*!
    \fn BWTcpPacket::GetWindow()
 */
unsigned short BWTcpPacket::GetWindow()
{
    if (!m_tcphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_tcphdr->window);
}


/*!
    \fn BWTcpPacket::GetChecksum()
 */
unsigned short BWTcpPacket::GetChecksum()
{
    if (!m_tcphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_tcphdr->check);
}


/*!
    \fn BWTcpPacket::GetUrgPointer()
 */
unsigned short BWTcpPacket::GetUrgPointer()
{
    if (!m_tcphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return ntohs(m_tcphdr->urg_ptr);
}


/*!
    \fn BWTcpPacket::GetOffset()
 */
unsigned short BWTcpPacket::GetOffset()
{
    if (!m_tcphdr) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo");
    }

    return m_tcphdr->doff;
}
