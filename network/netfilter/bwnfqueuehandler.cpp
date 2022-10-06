//
// C++ Implementation: bwnfqueuehandler
//
// Description: 
//
//
// Author: Samuel Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bwnfqueuehandler.h"

BWNfQueueHandler::BWNfQueueHandler(int qnum)
{
    m_id = qnum;
    m_queue_manager = NULL;
    m_queue_handler = NULL;
    m_netlink_handler = NULL;
    m_curpacket = NULL;
}


BWNfQueueHandler::~BWNfQueueHandler()
{
    if (m_queue_handler) {
        //nfq_destroy_queue(m_queue_handler);
    }

    //if (m_curpacket) {
    //    delete m_curpacket;
    //}
}


/*!
    \fn BWNfQueueHandler::SetQueue(struct nfq_q_handle *)
 */
void BWNfQueueHandler::SetQueue(struct nfq_q_handle *queue, 
			struct nfq_handle *manager)
{
    if (!queue || !manager) {
        throw BWError(BASE_ERROR, "Erro ao inicializar manipulador de fila");
    }

    m_queue_manager = manager;
    m_queue_handler = queue;

    if (nfq_set_mode(m_queue_handler, NFQNL_COPY_PACKET, 0xffff) < 0) {
        throw BWError(BASE_ERROR, "Erro ao setar modo de copia de pacote");
    }

    m_netlink_handler = nfq_nfnlh(m_queue_manager);
    m_queue = nfnl_fd(m_netlink_handler);
}


/*!
    \fn BWNfQueueHandler::NextPacket()
 */
BWNfqPacket *BWNfQueueHandler::NextPacket()
{
    fd_set sFds;
    int nRet;
    BWString oMsg;
    char buf[4096];

    FD_ZERO(&sFds);
    FD_SET(m_queue, &sFds);

    nRet = select(m_queue + 1, &sFds, NULL, NULL, NULL);

    if (nRet == -1) {
        oMsg = strerror(errno);

	throw BWError(SOCK_ERROR, oMsg);
    }

    nRet = recv(m_queue, buf, sizeof(buf), 0);

    if (nRet <= 0) {
        throw BWError(SOCK_ERROR, "Estouro da fila do kernel");
    }

    if (nRet > 0) {
        if (nfq_handle_packet(m_queue_manager, buf, nRet) != 0) {
            throw BWError(BASE_ERROR, "Erro ao executar nfq_handle_packet");
        }
    }

     return m_curpacket;
}


/*!
    \fn BWNfQueueHandler::HandlerCallback(struct nfq_q_handle *, struct nfgenmsg *, struct nfq_data *, void *)
 */
int BWNfQueueHandler::HandlerCallback(struct nfq_q_handle *qh, 
		struct nfgenmsg *nfmsg, struct nfq_data *nfa, void *data)
{
    BWNfQueueHandler *ptr = static_cast<BWNfQueueHandler *>(data);

    if (!nfa) {
        throw BWError(BASE_ERROR, "Erro ao obter nfq_data");
    }

    ptr->SetData(nfa);

    return 0;
}


/*!
    \fn BWNfQueueHandler::SetData(struct nfq_data *)
 */
void BWNfQueueHandler::SetData(struct nfq_data *data)
{
    if (!data) {
        throw BWError(BASE_ERROR, "Ponteiro de dados nulo (nfq_data)");
    }

    //if (m_curpacket) {
    //    delete m_curpacket;
    //      m_curpacket = NULL;
    //}

    m_curpacket = new BWNfqPacket();

    m_curpacket->SetData(data);
}


/*!
    \fn BWNfQueueHandler::SetDecision(int packet_id, int verdict)
 */
void BWNfQueueHandler::SetDecision(int packet_id, int verdict)
{
    int nRet;

    nRet = nfq_set_verdict(m_queue_handler, packet_id, verdict, 0, NULL);

    if (nRet < 0) {
        throw BWError(BASE_ERROR, "Erro ao setar veredito do pacote");
    }
}

int BWNfQueueHandler::GetQueueNum(void) {
    return m_id;
}

int BWNfQueueHandler::GetFd(void) {
    return m_queue;
}
