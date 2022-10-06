#include "bwksclient.h"

BWKSClient::BWKSClient(const BWString &cfg) {
    m_cfg.Load(cfg);

    m_sync = NULL;
    m_logger = NULL;

    init();
}

BWKSClient::BWKSClient(BWConfig &cfg) {
    m_cfg = cfg;

    m_sync = NULL;
    m_logger = NULL;
	
    init();
}

BWKSClient::~BWKSClient() {
    delete m_sync;
    delete m_logger;
}

bool BWKSClient::CheckWorkstation() {
	return m_sync->RegisterWorkstation();
}

BWString BWKSClient::GetDateTime() {
    return m_sync->GetDateTime();
}

void BWKSClient::init() {
	 try {
        m_sync = new BWKSSynchronizer(m_cfg);
        m_logger = new BWKSLogger(m_cfg);
    }
    catch (BWError &e) {
        m_log(BWLog::MSG_ERROR) << "Erro ao criar objetos do BWKeepSync: "
                                << e.GetErrorMessage() << bwlog::endl;
        throw e;
    }
}

void BWKSClient::run() {
    startSync();
    startLogger();

    m_sync->Wait();
    m_logger->Wait();

    delete this;
}

void BWKSClient::startSync() {
    try {
        m_sync->Start();
    }
    catch (BWError &e) {
        m_log(BWLog::MSG_ERROR) << "Erro ao iniciar objeto BWKSSynchronizer: "
                                << e.GetErrorMessage() << bwlog::endl;
        throw e;
    }
}

void BWKSClient::startLogger() {
    try {
        m_logger->Start();
    }
    catch (BWError &e) {
        m_log(BWLog::MSG_ERROR) << "Erro ao iniciar objeto BWKSLogger: " 
                                << e.GetErrorMessage() << bwlog::endl;
        throw e;
    }    
}

