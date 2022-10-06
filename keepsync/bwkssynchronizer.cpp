#include "bwkssynchronizer.h"

BWKSSynchronizer::BWKSSynchronizer(BWConfig &cfg) {
    m_cfg = cfg;

    try {
        init();
    }
    catch (BWError &e) {
        m_log(BWLog::MSG_ERROR) << "Erro ao iniciar objeto BWKSSynchronizer" 
                                << bwlog::endl;
        throw e;
    }
}

BWKSSynchronizer::~BWKSSynchronizer() {
}

void BWKSSynchronizer::init() {
    m_ksdb = bwdb_load(m_cfg["ksdb"]);
    m_appdb = bwdb_load(m_cfg["appdb"], true);

    m_transaction = new BWKSTransaction(m_cfg);
}

void BWKSSynchronizer::run() {
    while (true) {
		try {
			SyncRules();
		} 
		catch (BWError &e) {
			m_log(BWLog::MSG_INFO) << "Erro ao sincronizar regras: " 
								<< e.GetErrorMessage() << bwlog::endl;
		}

        exec_sleep(m_cfg["synctime"].GetInt());
    }
}

void BWKSSynchronizer::SyncRules() {
    BWList sections;

    m_log(BWLog::MSG_DEBUG) << "Iniciando sincronia de regras." << bwlog::endl;

    if (m_cfg["regras"] != "(null)" && m_cfg["regras"] != "") {
        sections = m_cfg["regras"].Split(",");

        for (unsigned int i = 0; i < sections.size(); i++) {
            syncSection(sections[i]);
        }
    }
}

void BWKSSynchronizer::syncSection(const BWString &section) {
    BWResultSet rset;
    BWKeyPair data;
    BWKeyPair upd;
    BWKeyPair where;
    BWString brange;
    BWString erange;
    BWString query = "SELECT ultimasincronia FROM bw_kssincronia WHERE ";

    m_log(BWLog::MSG_DEBUG) << "Sincronizando sessao: " << section 
                            << bwlog::endl;

    query += "sessao = '" + section + "'";

    rset = m_ksdb->ExecuteQuery(query);

    if (rset.GetNumRows() <= 0) {
        brange = "2000-01-01 00:00:00";
    }
    else {
        brange = rset.GetString("ultimasincronia");
    }

    if (!m_transaction->DoConnect()) {
        return;
    }

    erange = m_transaction->HasChanges(section, brange);

    if (erange == "") {
        m_transaction->Disconnect();
        return;
    }

    try {
        beginTransaction();

        processDeletes(section, brange, erange);
        processChanges(section, brange, erange);

        upd["ultimasincronia"] = erange;
        where["sessao"] = section;

        m_ksdb->Update("bw_kssincronia", upd, where);

        commitTransaction();
    }
    catch (BWError &e) {
        m_log(BWLog::MSG_ERROR) << "Erro ao sincronizar regras: "
                                << e.GetErrorMessage() << bwlog::endl;

        rollbackTransaction();
    }

    m_transaction->Disconnect();
}

void BWKSSynchronizer::processDeletes(const BWString &section,
                       const BWString &brange, const BWString &erange) {
    BWKeyPair data;

    m_transaction->BeginDeletes(section, brange, erange);

    while (m_transaction->GetNextDelete(&data)) {
        deleteToDatabase(data);
    }

    m_transaction->EndDeletes(section, "OK");
}

void BWKSSynchronizer::processChanges(const BWString &section,
                       const BWString &brange, const BWString &erange) {
    BWKeyPair data;

    m_transaction->BeginChanges(section, brange, erange);

    while (m_transaction->GetNextChange(&data)) {
        writeToDatabase(data);
    }

    m_transaction->EndChanges(section, "OK");
}

void BWKSSynchronizer::writeToDatabase(BWKeyPair data) {
    BWString idfield = "id";
    BWString query;
    BWResultSet rset;
    BWKeyPair record;
    BWKeyPair where;
    BWList fields;

    fields = data["fields"].Split(",");
    record = data.GetSubKeyPair(fields);

    if (data["relation"] != "none") {
        m_appdb->Insert(data["table"], record);
        return;
    }

    if (data["idfield"] != "(null)") {
        idfield = data["idfield"];
    }

    query  = "SELECT " + idfield + " FROM " + data["table"];
    query += " WHERE " + idfield + " = '" + data[idfield] + "'";

    rset = m_appdb->ExecuteQuery(query);

    if (rset.GetNumRows() > 0) {
        where[idfield] = record[idfield];
        m_appdb->Update(data["table"], record, where);
    }
    else {
        m_appdb->Insert(data["table"], record);
    }
}

void BWKSSynchronizer::deleteToDatabase(BWKeyPair data) {
    BWKeyPair where;
    BWList wherefields;

    wherefields = data["wherefields"].Split(",");

    for (unsigned int i = 0; i < wherefields.size(); i++) {
        where[wherefields[i]] = data[wherefields[i]];
    }

    m_appdb->Delete(data["table"], where);
}

void BWKSSynchronizer::beginTransaction() {
    m_log(BWLog::MSG_XDEBUG) << "Iniciando transacao em banco de dados"
                             << bwlog::endl;

    m_ksdb->BeginTransaction();
    m_appdb->BeginTransaction();
}

void BWKSSynchronizer::commitTransaction() {
    m_log(BWLog::MSG_XDEBUG) << "Fazendo commit em banco de dados" 
                             << bwlog::endl;

    m_ksdb->Commit();
    m_appdb->Commit();
}

void BWKSSynchronizer::rollbackTransaction() {
    m_log(BWLog::MSG_XDEBUG) << "Fazendo rollback em banco de dados" 
                             << bwlog::endl;

    m_ksdb->Rollback();
    m_appdb->Rollback();
}

bool BWKSSynchronizer::RegisterWorkstation() {
    BWString query = "SELECT id FROM bw_estacao LIMIT 1";
    BWResultSet rset;
    BWKeyPair record;
    BWString estacao;
	
    rset = m_appdb->ExecuteQuery(query);

    if (rset.GetNumRows() > 0) {
        return true;
    }

    m_transaction->DoConnect();
    //estacao = m_transaction->RegisterWorkstation(BWWinUtils::GetPCName());
    estacao = m_transaction->RegisterWorkstation(get_pc_name());
	
    if (estacao == "") {
        return false;
    }
	
    record["id"] = estacao;
	
    m_appdb->Insert("bw_estacao", record);
	
    m_transaction->Disconnect();
	
    return true;
}

BWString BWKSSynchronizer::GetDateTime() {
    BWString dt = "";
	
	if (m_transaction->DoConnect()) {
        dt = m_transaction->GetDateTime();
	    m_transaction->Disconnect();
	}
	
	return dt;
}
