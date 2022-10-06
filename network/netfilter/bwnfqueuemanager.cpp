//
// C++ Implementation: bwnfqueue
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bwnfqueuemanager.h"

BWNfQueueManager::BWNfQueueManager()
{
    m_handler = NULL;
    m_initialized = false;
}


BWNfQueueManager::~BWNfQueueManager()
{
    if (m_handler) {
        if (m_initialized) {
            //nfq_unbind_pf(m_handler, AF_INET);
        }
        nfq_close(m_handler);
    }
}


/*!
    \fn BWNfQueueManager::Init()
 */
void BWNfQueueManager::Init()
{
    m_handler = nfq_open();

    if (!m_handler) {
        throw BWError(BASE_ERROR, "Erro durante nfq_open()");
    }

    if (nfq_unbind_pf(m_handler, AF_INET) < 0) {
        // fazer log em caso de erro
    }

    m_initialized = false;

    if (nfq_bind_pf(m_handler, AF_INET) < 0) {
        throw BWError(BASE_ERROR, "Erro durante nfq_bind_pf()");
    }

    m_initialized = true;
}


/*!
    \fn BWNfQueueManager::CreateQueue(int qnum)
 */
BWNfQueueHandler *BWNfQueueManager::CreateQueue(int qnum)
{
    struct nfq_q_handle *queue;
    BWNfQueueHandler *qhandler = new BWNfQueueHandler(qnum);

    queue = nfq_create_queue(m_handler, qnum, &(BWNfQueueHandler::HandlerCallback), qhandler);

    if (!queue) {
        throw BWError(BASE_ERROR, "Erro ao criar fila");
    }

    qhandler->SetQueue(queue, m_handler);

    return qhandler;
}
