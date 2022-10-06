//
// C++ Interface: bwlogger
//
// Description: 
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWLOGGER_H
#define BWLOGGER_H

#include <bwthread.h>
#include <bwdbregistry.h>
#include <bwstring.h>
#include <bwdatabase.h>
#include <unistd.h>
#include <iostream>
#include <queue>

using std::queue;

/**
Classe para manipular logs de transacoes no banco de dados.

	@author Samuel da Silva Feitosa <samuel@beanweb.com.br>
*/
class BWLogger: public BWThread {
public:
    BWLogger(queue<BWDbRegistry> *queueLog, const BWString &rcpath);
    ~BWLogger();

    void run();
    void logger();

private:
    queue<BWDbRegistry> *m_log;
    BWDb *m_db;

};

#endif

