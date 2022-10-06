//
// C++ Interface: bwnfqueue
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWNFQUEUEMANAGER_H
#define BWNFQUEUEMANAGER_H

extern "C" {
#include <libnetfilter_queue/libnetfilter_queue.h>
}
#include <bwstring.h>
#include <bwnfqueuehandler.h>


/**
Classe responsavel pela manipulacao das estruturas da libnetfilter_queue.

	@author Samuel Feitosa <samuel@beanweb.com.br>
*/
class BWNfQueueManager {
public:
    BWNfQueueManager();
    ~BWNfQueueManager();

    void Init();
    BWNfQueueHandler *CreateQueue(int qnum);

private:
    nfq_handle *m_handler;
    bool m_initialized;
};

#endif
