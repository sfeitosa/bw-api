//
// C++ Implementation: bwlogger
//
// Description: 
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bwlogger.h"

BWLogger::BWLogger(queue<BWDbRegistry> *queueLog, const BWString &rcpath) : BWThread()
{
	m_log = queueLog;
	m_db = bwdb_load(rcpath);
}

BWLogger::~BWLogger()
{
}

/*!
    \fn BWLogger::run()
 */
void BWLogger::run()
{
	int sleepCur = 0;
	int sleepInc = 5;
	int sleepMax = 60;

	cout << "Iniciando thread (BWLogger)" << endl;

	while (true) {
		if (m_log->size() > 0) {
			logger();
			sleepCur = 0;
		}
		else {
			sleepCur += sleepInc;
			
			if (sleepCur > sleepMax) {
				sleepCur = 0;
			}
		}

		cout << "(BWLogger) Nada para logar. Aguardando " << sleepCur << " segundos..." << endl;
		sleep(sleepCur);
	}

	cout << "Finalizando thread (BWLogger)" << endl;
}

/*!
    \fn BWLogger::logger()
 */
void BWLogger::logger() {
	while (!m_log->empty()) {
		try {
			m_log->front().SetDb(*m_db);
			m_log->front().Save();
		}
		catch(BWError &e) {
			e.TrackError(m_BWErrorPlace, "Erro ao gravar registro no log");
			cerr << "Erro ao gravar registro no log" << endl;
		}

		m_log->pop();
	}
}
