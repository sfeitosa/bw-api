#include <bwauthorize.h>
#include <iostream>

using namespace std;

#define out_debug(msg) (cerr << m_pidc << ": " << msg << endl);

BWAuthorize::BWAuthorize(BWAcceptedServer *conn, BWConfig cfg, 
		BWTemplate *sqlUser, BWTemplate *sqlQryCache, 
		BWTemplate *sqlQuery, queue<BWDbRegistry> *q) {

	m_conn = conn;
	m_db = NULL;
	m_sqlUser = sqlUser;
	m_sqlQryCache = sqlQryCache;
	m_sqlQuery = sqlQuery;
	m_cfg = cfg;
	m_pidc = 0;
	m_queue = q;

	startUp();
}

BWAuthorize::~BWAuthorize(void) {
}

void BWAuthorize::startUp(void) {
	try {
        	m_db = bwdb_load(m_cfg["db_cfg"]);
	}
	catch(BWError &e) {
		e.TrackError(m_BWErrorPlace, "Erro ao carregar resource de banco de dados");
		throw;
	}
}

void BWAuthorize::run(void) {
	BWString readdata;
	BWKeyPair oInput;
	BWString user;
	BWString oSend;
	BWKeyPair data;

	cout << "Iniciando thread (BWAuthorize)" << endl;

	do {
		cout << "Aguardando dados" << endl;
		readdata = m_conn->ReadUntil("\n");

		if (readdata.size() > 2) {
			cout << "Recebido: " << readdata << endl;
			oInput.FromString(readdata, "=", " ", "\"");

			m_pidc = oInput.GetInt("__pidc");

			data = authorize(oInput);

			logger(data);
		}

	} while(readdata.size() > 0);

	cout << "Finalizando thread (BWAuthorize)" << endl;
}

BWKeyPair BWAuthorize::authorize(BWKeyPair &inputData) {
	BWResultSet rset;
	BWString oResult;
	BWString user;
	BWKeyPair tmp;
	BWKeyPair ret = inputData;
	int nBytes = 0;
	bool fromcache = false;
	
	user = getUser(inputData);

	out_debug("Usuario encontrado: [" << user << "]");

	if (user != "") {
		out_debug("Consulting cache...");
		rset = doAuth(m_sqlQryCache, user, inputData);

		out_debug("Return from doAuth()");

		if (rset.GetNumRows() > 0) {
			fromcache = true;
			out_debug("Return from cache...");
			oResult = formatReturn(rset, user, "S");
		}
		else {
			out_debug("Consulting base...");
			rset = doAuth(m_sqlQuery, user, inputData);
			
			oResult = formatReturn(rset, user, "N");
		}
	}
	else {
		oResult = formatReturn(user);
	}

	// Respondendo requisicao de autorizacao
	out_debug("Retornando atraves do socket: " << oResult);
	nBytes = m_conn->Send(oResult);
	out_debug("Bytes enviados: [" << nBytes << "]");

	tmp = rset.GetTuple();
	ret.AppendKeyPair(tmp);

	ret["usuario_id"] = user;

	if (m_cfg["cache_keywords"].size() > 0 && !fromcache && user != "") {
		updateCache(ret);
	}

	return ret;
}

BWResultSet BWAuthorize::doAuth(BWTemplate *sql, const BWString &user_id, BWKeyPair &inputData) {
	BWResultSet rset;
	BWString oQuery;

	out_debug("doAuth::[" << inputData.ToString() << "]");

	try {
		oQuery = makeQuery(sql, user_id, inputData);
		out_debug("Autorizando: " << oQuery);
		rset = m_db->ExecuteQuery(oQuery);
		out_debug("Query executada com sucesso!");
	}
	catch (BWError &e) {
		out_debug("Erro: " << e.GetErrorMessage());
		//oResult = formatReturn(user);
	}

	out_debug("Retornando rset");

	return rset;
}

void BWAuthorize::updateCache(BWKeyPair &data) {
	BWList cachekeywords = m_cfg["cache_keywords"].Split(",");
	BWDbRegistry registry(*m_db, "bwt_cachewm", cachekeywords);
	BWKeyPair cachedata = data.GetSubKeyPair(cachekeywords);

	out_debug("BWAuthorize::updateCache()");

	out_debug("CacheData: [" << cachedata.ToString() << "]");

	try {
		registry.Create();
		registry.Set(cachedata);
		out_debug("Salvando registro de cache...");
		registry.Save();
		out_debug("OK");
	}
	catch (BWError &e) {
		e.TrackError(m_BWErrorPlace, "Erro ao inserir cache");
		cerr << "Erro gravando cache: " << e.GetErrorMessage() << endl;
	}
}

void BWAuthorize::logger(BWKeyPair &data) {
	BWList logkeywords = m_cfg["logger_keywords"].Split(",");
	BWDbRegistry registry(*m_db, "bw_logwm", logkeywords);
	BWKeyPair logdata = data.GetSubKeyPair(logkeywords);

	out_debug("BWAuthorize::logger()");

	out_debug("LOGDATA: [" << logdata.ToString() << "]");

	try {
		registry.Create();
		registry.Set(logdata);
	} catch (BWError &e) {
		e.TrackError(m_BWErrorPlace, "Erro ao inserir log");
		out_debug("Erro gravando log: " << e.GetErrorMessage());
	}

	m_queue->push(registry);
}

BWString BWAuthorize::getUser(BWKeyPair &kp) {
	BWKeyPair attr;
	BWResultSet rset;

	m_sqlUser->ClearAttributes();

	attr["iporigem"] = kp["iporigem"];
	attr["user"] = kp["user"];

	m_sqlUser->SetAttributes(attr);

	out_debug("Buscando usuario: " << m_sqlUser->GetText());

	try {
		rset = m_db->ExecuteQuery(m_sqlUser->GetText());
	} catch (BWError &e) {
		cerr << "Erro: " << e.GetErrorMessage() << endl;
	}

	if (rset.GetNumRows() > 0) {
		return rset.GetString("usuario_id");
	}

	return "";
}

BWString BWAuthorize::makeQuery(BWTemplate *qry, const BWString &user_id, BWKeyPair &kp) {
	BWList keywords;

	qry->ClearAttributes();

	kp["usuario_id"] = user_id;

	qry->SetAttributes(kp);

	return qry->GetText();
}

BWString BWAuthorize::formatReturn(BWResultSet rset, const BWString &user, 
				const BWString &fromcache) {
	BWKeyPair oReturn;
	BWList keywords;
	BWString strFormatted;

	oReturn["__usuario_id"] = user;
	oReturn["__from_cache"] = fromcache;

	if (rset.GetNumRows() > 0) {
		keywords = m_cfg["output_keywords"].Split(",");
	
		for (unsigned int i = 0; i < keywords.size(); i++) {
			oReturn[keywords[i]] = rset.GetString(keywords[i]);
		}
	}
	else {
		oReturn["acao"] = "N";
	}

	strFormatted = oReturn.ToString("=", " ", "\"") + "\n";

	return strFormatted;
}

BWString BWAuthorize::formatReturn(const BWString &user) {
	BWString retstr = "__usuario_id=\"" + user + "\" __from_cache=\"N\" acao=\"N\"\n";

	return retstr;
}
