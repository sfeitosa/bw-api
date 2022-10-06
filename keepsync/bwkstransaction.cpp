#include <bwkstransaction.h>

#include <iostream>

using namespace std;

BWKSTransaction::BWKSTransaction(BWConfig &cfg) {
    m_cfg = cfg;
    m_client = NULL;
}

BWKSTransaction::~BWKSTransaction() {
    if (m_client) {
        delete m_client;
    }
}

bool BWKSTransaction::DoConnect() {
    bool connected = true;

    m_log(BWLog::MSG_DEBUG) << "Conectando no KeepSync Server" << bwlog::endl;

	if (m_client == NULL) {
        try {
            m_client = new BWClient(m_cfg["ip"], m_cfg.GetInt("port"), 
                                    m_cfg.GetInt("timeout"));
        }
        catch (BWError &e) {
            m_log(BWLog::MSG_WARN) << "Erro ao criar objeto BWClient: "
                                   << e.GetErrorMessage() << bwlog::endl;
            m_client = NULL;
            return false;
        }
	}

    try {
        m_client->Open();
        m_client->Connect();
    }
    catch (BWError &e) {
        m_log(BWLog::MSG_WARN) << "Erro ao conectar: " 
                               << e.GetErrorMessage() << bwlog::endl;

        if (m_client->IsOpen()) {
            m_client->Close();
        }

        connected = false;
    }

    return connected;
}

void BWKSTransaction::Disconnect() {
    if (m_client->IsOpen()) {
        m_client->Close();
    }
}

BWString BWKSTransaction::HasChanges(const BWString &section, const BWString &lastsync) {
    BWKeyPair snd;
    BWKeyPair rsp;

    snd["message"] = "haschange";
    snd["action"] = "get";
    snd["section"] = section;
    snd["lastsync"] = lastsync;

    rsp = Chat(snd);

    if (rsp["response"] == "true") {
        return rsp["lastchange"];
    }

    return "";    
}

bool BWKSTransaction::BeginChanges(const BWString &section, 
                                   const BWString &brange,
                                   const BWString &erange) {
    BWKeyPair snd;
    BWKeyPair rsp;

    snd["message"] = "changes";
    snd["action"] = "get";
    snd["section"] = section;
    snd["beginrange"] = brange;
    snd["endrange"] = erange;

    rsp = Chat(snd);

    if (rsp["action"] == "begin") {
        return true;
    }

    return false;
}

void BWKSTransaction::EndChanges(const BWString &section, 
                                 const BWString &status) {
    BWKeyPair snd;
    BWKeyPair rsp;

    snd["message"] = "changes";
    snd["action"] = "get";
    snd["section"] = section;
    snd["status"] = status;

    rsp = Chat(snd);
}

bool BWKSTransaction::GetNextChange(BWKeyPair *rsp) {
    BWString data;
	BWKeyPair recv;
	BWKeyPair snd;

    data = m_client->ReadUntil("\n");
	
	recv.FromString(data);

    if (recv["action"] == "cont") {
	    snd["status"] = "OK";
        snd["action"] = "cont";
		
		recv = Chat(snd);
	}

    rsp->clear();
    rsp->AppendKeyPair(recv);

    if (rsp->GetString("action") == "end") {
        return false;
    }

   return true;
}

bool BWKSTransaction::BeginDeletes(const BWString &section, 
                                   const BWString &brange,
                                   const BWString &erange) {
    BWKeyPair snd;
    BWKeyPair rsp;

    snd["message"] = "deletes";
    snd["action"] = "get";
    snd["section"] = section;
    snd["beginrange"] = brange;
    snd["endrange"] = erange;

    rsp = Chat(snd);

    if (rsp["action"] == "begin") {
        return true;
    }

    return false;
}

void BWKSTransaction::EndDeletes(const BWString &section, 
                                 const BWString &status) {
    BWKeyPair snd;
    BWKeyPair rsp;

    snd["message"] = "deletes";
    snd["action"] = "get";
    snd["section"] = section;
    snd["status"] = status;

    rsp = Chat(snd);
}

bool BWKSTransaction::GetNextDelete(BWKeyPair *rsp) {
    BWString data;
	BWKeyPair recv;
	BWKeyPair snd;

    data = m_client->ReadUntil("\n");

    recv.FromString(data);
	
	if (recv["action"] == "cont") {
	    snd["status"] = "OK";
        snd["action"] = "cont";
		
		recv = Chat(snd);
	}

    rsp->clear();
    rsp->AppendKeyPair(recv);

    if (rsp->GetString("action") == "end") {
        return false;
    }

   return true;
}

BWKeyPair BWKSTransaction::Chat(BWKeyPair kpr) {
    BWKeyPair rsp;
    BWString message;
    BWString response;

    message = kpr.ToString();
	
	// Retirada de quebras de linha para corrigir problemas de versoes
	// antigas do software
	message.Replace("\n", " ");
	
    message += "\n";

    m_log(BWLog::MSG_XDEBUG) << "Chat::Enviando: " << message << bwlog::endl;

    m_client->Send(message);

    m_log(BWLog::MSG_XDEBUG) << "Chat -> dados enviados..." << bwlog::endl;

    response = m_client->ReadUntil("\n");

    m_log(BWLog::MSG_XDEBUG) << "Chat -> Passou do ReadUntil" << bwlog::endl;

    if (response.length() < 2) {
        throw BWError(SOCK_ERROR, "Erro ao receber dados");
    }

    m_log(BWLog::MSG_XDEBUG) << "Chat::Recebido: " << response << bwlog::endl; 

    rsp.FromString(response);

    return rsp;
}

bool BWKSTransaction::BeginLogs(const BWString &section) {
    BWKeyPair snd;
    BWKeyPair rsp;

    snd["message"] = "logs";
    snd["action"] = "send";
    snd["section"] = section;

    rsp = Chat(snd);

    if (rsp["status"] == "OK") {
        return true;
    }

    return false;
}

void BWKSTransaction::EndLogs(const BWString &section) {
    BWKeyPair snd;
    BWKeyPair rsp;

    snd["message"] = "logs";
    snd["action"] = "end";
    snd["status"] = "OK";
    snd["section"] = section;

    rsp = Chat(snd);
}

void BWKSTransaction::SendLog(BWKeyPair data) {
    BWString message = data.ToString();
	
	// Retirada de quebras de linha para corrigir problemas de versoes
	// antigas do software
	message.Replace("\n", " ");
	
    message += "\n";

    m_log(BWLog::MSG_XDEBUG) << "SendLog::Enviando: " << message << bwlog::endl;

    m_client->Send(message);    
}

BWString BWKSTransaction::RegisterWorkstation(const BWString &computer) {
    BWKeyPair snd;
	BWKeyPair rsp;

    snd["message"] = "estacao";
    snd["action"] = "get";
    snd["computer"] = computer;

    rsp = Chat(snd);

    if (rsp["status"] == "OK") {
        return rsp["estacao_id"];
    }

    return "";
}

BWString BWKSTransaction::GetDateTime() {
    BWKeyPair snd;
    BWKeyPair rsp;
    
    snd["message"] = "datahora";
    snd["action"] = "get";
    
    rsp = Chat(snd);
    
    if (rsp["status"] == "OK") {
        return rsp["datahora"];
    }
    
    return "";
}
