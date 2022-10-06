//
// C++ Interface: bwnfqueuehandler
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWNFQUEUEHANDLER_H
#define BWNFQUEUEHANDLER_H

extern "C" {
#include <libnetfilter_queue/libnetfilter_queue.h>
}

#include <bwstring.h>
#include <bwnfqpacket.h>
#include <errno.h>
#include <linux/netfilter.h>
#include <iostream>

using namespace std;

/**
Classe responsavel pela manipulacao das estruturas nfq_q_handler.

	@author Samuel Feitosa <samuel@beanweb.com.br>
*/
class BWNfQueueHandler{
public:
    BWNfQueueHandler(int qnum);

    ~BWNfQueueHandler();
    void SetQueue(struct nfq_q_handle *, struct nfq_handle *);
    BWNfqPacket *NextPacket();
    void SetData(struct nfq_data *);
    void SetDecision(int packet_id, int verdict);
    static int HandlerCallback(struct nfq_q_handle *, struct nfgenmsg *, 
			struct nfq_data *, void *);
    int GetQueueNum(void);
    int GetFd(void);

private:
    int m_queue;
    int m_id;
    struct nfq_handle *m_queue_manager;
    struct nfq_q_handle *m_queue_handler;
    struct nfnl_handle *m_netlink_handler;
    BWNfqPacket *m_curpacket;
};

#endif
