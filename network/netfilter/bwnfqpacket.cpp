//
// C++ Implementation: bwnfqpacket
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bwnfqpacket.h"

BWNfqPacket::BWNfqPacket()
{
    m_pktheader = NULL;
    m_nfqdata = NULL;
    m_payload = NULL;
    m_payloadlen = 0;
}


BWNfqPacket::~BWNfqPacket()
{
}

/*!
    \fn BWNfqPacket::GetId()
 */
int BWNfqPacket::GetId()
{
    return ntohl(m_pktheader->packet_id);
}


/*!
    \fn BWNfqPacket::GetProtocol()
 */
short BWNfqPacket::GetProtocol()
{
    return ntohs(m_pktheader->hw_protocol);
}


/*!
    \fn BWNfqPacket::GetIndev
 */
int BWNfqPacket::GetInDevice()
{
    return nfq_get_indev(m_nfqdata);
}


/*!
    \fn BWNfqPacket::GetOutDevice()
 */
int BWNfqPacket::GetOutDevice()
{
    return nfq_get_outdev(m_nfqdata);
}


/*!
    \fn BWNfqPacket::init()
 */
void BWNfqPacket::loadHeader()
{
    m_pktheader = nfq_get_msg_packet_hdr(m_nfqdata);

    if (!m_pktheader) {
        throw BWError(BASE_ERROR, "Erro ao obter header do pacote");
    }
}


/*!
    \fn BWNfqPacket::GetHook()
 */
#include <stdio.h>
char BWNfqPacket::GetHook()
{
    if (m_pktheader->hook) {
       printf("TEM: %d\n", m_pktheader->hook);
    }
    else {
       printf("Nao tem\n");
    }

    return m_pktheader->hook;
}

/*!
    \fn BWNfqPacket::GetPayloadLen()
 */
int BWNfqPacket::GetPayloadLen()
{
    return m_payloadlen;
}

/*!
    \fn BWNfqPacket::GetHookName()
 */
BWString BWNfqPacket::GetHookName()
{
    BWString chain;

    switch (GetHook()) {
        case NF_INET_PRE_ROUTING: {
            chain = "PREROUTING";
            break;
        }
        case NF_INET_LOCAL_IN: {
            chain = "INPUT";
            break;
        }
        case NF_INET_FORWARD: {
            chain = "FORWARD";
            break;
        }
        case NF_INET_LOCAL_OUT: {
            chain = "OUTPUT";
            break;
        }
        case NF_INET_POST_ROUTING: {
            chain = "POSTROUTING";
            break;
        }
        default: {
            chain = "UNKNOWN";
            break;
        }
    }

    return chain;
}


/*!
    \fn BWNfqPacket::GetPayload()
 */
unsigned char * BWNfqPacket::GetPayload()
{
    if (!m_payload) {
        m_payloadlen = 0;
        m_payloadlen = nfq_get_payload(m_nfqdata, &m_payload);
    }

    if (m_payloadlen < sizeof(struct iphdr)) {
        throw BWError(BASE_ERROR, 
         "Erro ao obter payload do pacote (tamanho invalido para pacote IP)");
    }

    return m_payload;
}


/*!
    \fn BWNfqPacket::SetData(struct nfq_data *)
 */
void BWNfqPacket::SetData(struct nfq_data *data)
{
    m_nfqdata = data;

    if (!m_nfqdata) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo (nfq_data)");
    }

    loadHeader();
}
