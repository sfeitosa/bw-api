//
// C++ Interface: bwnfqpacket
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWNFQPACKET_H
#define BWNFQPACKET_H
extern "C" {
#include <libnetfilter_queue/libnetfilter_queue.h>
}
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <bwerror.h>
#include <bwstring.h>

/**
Classe responsavel por manipular as estruturas de dados da Netfilter Queue e retornar os dados necessarios.

	@author Samuel Feitosa <samuel@beanweb.com.br>
*/
class BWNfqPacket{
public:
    enum NfInetHooks {
        NF_INET_PRE_ROUTING,
        NF_INET_LOCAL_IN,
        NF_INET_FORWARD,
        NF_INET_LOCAL_OUT,
        NF_INET_POST_ROUTING,
        NF_INET_NUMHOOKS
    };

    BWNfqPacket();
    ~BWNfqPacket();
    int GetId();
    short GetProtocol();
    int GetInDevice();
    int GetOutDevice();
    char GetHook();
    BWString GetHookName();
    unsigned char *GetPayload();
    int GetPayloadLen();
    void SetData(struct nfq_data *);

private:
    void loadHeader();

    struct nfq_data *m_nfqdata;
    struct nfqnl_msg_packet_hdr *m_pktheader;
    unsigned char *m_payload;
    unsigned int m_payloadlen;
};

#endif
