#include <bwauthhandler.h>
#include <iostream>

using namespace std;

BWAuthHandler::BWAuthHandler(BWAcceptedServer *conn,
			BWConfig cfg, BWTemplate *sqluser, BWTemplate *sqlqrycache,
			BWTemplate *sqlquery, BWTemplate *sqllogger) {
	m_conn = conn;
	m_db = NULL;
	m_sqlUser = sqluser;
	m_sqlQryCache = sqlqrycache;
	m_sqlQuery = sqlquery;
	m_sqlLogger = sqllogger;
	m_cfg = cfg;

	startUp();
}

void BWAuthHandler::startUp(void) {
    try {
        m_db = bwdb_load(m_cfg["db_cfg"]);
    }
    catch(BWError &e) {
        e.TrackError(m_BWErrorPlace, "Erro ao carregar resource de banco de dados");
        throw;
    }
}

void BWAuthHandler::run(void) {
	BWString readdata;
	BWAuthorize *auth;
	BWKeyPair oInput;
	BWString user;
	BWString oSend;

	cout << "Iniciando thread (BWAuthHandler)" << endl;

	do {
		cout << "Aguardando dados" << endl;
		readdata = m_conn->ReadUntil("\n");

		if (readdata.size() > 2) {
			cout << "Recebido: " << readdata << endl;
			oInput.FromString(readdata, "=", " ", "\"");

			auth = new BWAuthorize(m_conn, m_db, m_cfg, oInput,
					m_sqlUser, m_sqlQryCache, m_sqlQuery, m_sqlLogger);

			auth->Start();
			auth->Detach();
		}

	} while(readdata.size() > 0);

	cout << "Finalizando thread (BWAuthHandler)" << endl;
}
