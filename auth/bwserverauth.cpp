#include <bwserverauth.h>
#include <iostream>

using namespace std;

BWServerAuth::BWServerAuth(const BWString &cfg) {
    loadCfg(cfg);
    startUp();
    loadSql();
}

BWServerAuth::~BWServerAuth(void) {
    m_logger->Detach();
}

void BWServerAuth::startUp(void) {
    m_server = new BWServer(m_cfg["ip"], m_cfg.GetInt("port"),
                            m_cfg.GetInt("timeout"));

    m_logger = new BWLogger(&m_queue, m_cfg["db_cfg"]);
    m_logger->Start();
}

void BWServerAuth::WaitConn(void) {
    BWAcceptedServer *sock_client;
    BWAuthorize *cli_handler;

    while (true) {
        cout << "Aguardando conexao em " << m_cfg["ip"] << ":" << m_cfg["port"] << endl;
        sock_client = m_server->Accept();

        cli_handler = new BWAuthorize(sock_client, m_cfg, m_sqlUser, 
				m_sqlQryCache, m_sqlQuery, &m_queue);

        cli_handler->Start();
        cli_handler->Detach();
    }
}

void BWServerAuth::loadCfg(const BWString &cfgpath) {
    m_cfg.Load(cfgpath);

    if (m_cfg["ip"] == "") {
        m_cfg["ip"] = "0.0.0.0";
    }

    if (m_cfg["port"] == "") {
        m_cfg["port"] = "33500";
    }

    if (m_cfg["timeout"] == "") {
        m_cfg["timeout"] = "60";
    }

    if (m_cfg["sqlauth"] == "") {
        throw BWError(APP_ERROR, "Deve-se informar o caminho para o arquivos de SQL");
    }

    if (m_cfg["db_cfg"] == "") {
        throw BWError(DB_ERROR, "Deve-se informar o caminho para a configuracao do banco de dados");
    }
}

void BWServerAuth::loadSql(void) {
    m_sqlUser = new BWTemplate();
    m_sqlUser->FromFile(m_cfg["sqluser"]);

    m_sqlQryCache = new BWTemplate();
    m_sqlQryCache->FromFile(m_cfg["sqlcache"]);

    m_sqlQuery = new BWTemplate();
    m_sqlQuery->FromFile(m_cfg["sqlauth"]);
}
