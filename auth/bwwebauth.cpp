#include <bwwebauth.h>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

BWWebAuth::BWWebAuth(const BWString &cfg) {
	loadCfg(cfg);

	m_pid = getpid();
}

BWWebAuth::~BWWebAuth(void) {
	delete m_client;
}

void BWWebAuth::Run(void) {
	cerr << m_pid << ": BWWebAuth::Run()" << endl;
	startUp();

	waitInputData();
}

void BWWebAuth::loadCfg(const BWString &cfg) {
	m_cfg.Load(cfg);

	/// @todo Validar parametros de entrada
	if (m_cfg["ip"] == "") {
		m_cfg["ip"] = "127.0.0.1";
	}

	if (m_cfg["port"] == "") {
		m_cfg["port"] = "33500";
	}

	if (m_cfg["timeout"] == "") {
		m_cfg["timeout"] = "10";
	}

	if (m_cfg["local_error_page"] == "") {
		throw BWError(APP_ERROR, "Pagina de erro padrao nao informada.");
	}

	if (m_cfg["local_login_page"] == "") {
		throw BWError(APP_ERROR, "Pagina de login padrao nao informada.");
	}
}

void BWWebAuth::startUp(void) {
	m_client = new BWClient(m_cfg["ip"], m_cfg.GetInt("port"),
				m_cfg.GetInt("timeout"));
}

void BWWebAuth::doConnect(void) {
	do {
		cerr << m_pid << ": Conectando em " << m_cfg["ip"] <<
			":" << m_cfg["port"] << "." << endl;
		try {
			m_client->Open();
			m_client->Connect();
		}
		catch (BWError &e) {
			cerr << m_pid << ": Erro a conectar: " << e.GetErrorMessage() << endl;
			sleep(5);
		}
	} while (!m_client->IsOpen());
}

void BWWebAuth::waitInputData(void) {
	BWString input;
	BWString output;
	BWString oInput;
	BWString readdata;

	cerr << m_pid << ": BWWebAuth::waitInputData()" << endl;

	while (true) {

		doConnect();

		do {
			cerr << m_pid << ": Aguardando dados" << endl;

			getline(cin, input);

			if (cin.eof()) {
				break;
			}

			cerr << m_pid << ": Lido: [" << input << "]" << endl;

			oInput = formatInput(input);

			if (oInput.length() == 0) {
				cout << m_cfg["local_error_page"] << endl;
				continue;
			}

			cerr << m_pid << ": Enviando: [" << oInput << "]" << endl;

			m_client->Send(oInput);

			readdata = m_client->ReadUntil("\n");

			cerr << m_pid << ": Lido do socket: " << readdata << endl;

			if (readdata.length() > 0) {
				/// @todo Melhorar isso aqui
				output = formatOutput(readdata, input.Split(" ")[0]);

				cout << output << endl;
			}
			else {
				/// @todo Ver o que da para fazer aqui
				cerr << m_pid << ": Retornando pagina de erro" << endl;
				cout << m_cfg["local_error_page"] << endl;
			}

		} while (!cin.eof() && m_client->IsOpen());

		if (!m_client->IsOpen()) {
			m_client->Close();
		}

		if (cin.eof()) {
			cerr << m_pid << ": cin.eof()" << endl;
			break;
		}
	}
}

BWString BWWebAuth::formatInput(const BWString &input) {
	BWKeyPair oInput;
	BWList tokens;
	BWString ret;
	BWString cachehash;
	BWString pid;

	pid.AppendInt(m_pid);
	
	tokens = input.Split(" ");

	try {
		BWUrl oUrl(tokens[0]);
		oInput["host"] = oUrl.GetHost();
		oInput["web_host_select_in"] = oUrl.GetDomainSQLList();
	}
	catch (BWError &e) {
		e.TrackError(m_BWErrorPlace, BWString("Erro ao criar objeto para a url [") +
					tokens[0] + BWString("]"));
		cerr << "Erro ao criar objeto para a url [" << tokens[0] << "]" << endl;
		return "";
	}
	
	oInput["__pidc"] = pid;
	oInput["url"] = tokens[0];
	oInput["iporigem"] = tokens[1].Split("/")[0];

	if (tokens[2] == "-") {
		oInput["user"] = "";
	}
	else {
		oInput["user"] = tokens[2];
	}

	cerr << "User: " << oInput["user"] << endl;

	cachehash = "teste";
	cachehash = oInput["iporigem"] + oInput["host"] + oInput["user"];
	oInput["cache_hash"] = cachehash.Hash();

	ret = oInput.ToString("=", " ", "\"") + "\n";

	return ret;
}

BWString BWWebAuth::formatOutput(const BWString &readdata, const BWString &urlIn) {
	BWKeyPair oOutput;
	BWString strUrl;

	oOutput.FromString(readdata, "=", " ", "\"");

	if (oOutput["__usuario_id"] == "") {
		strUrl = m_cfg["local_login_page"];
	}
	else if (oOutput["acao"] == "P") {
		strUrl = urlIn;
	}
	else {
		strUrl = m_cfg["local_error_page"];
		/// @todo Colocar a mensagem de erro e o retorno
		// da regra
	}

	cerr << m_pid << ": formatOutput::strUrl: " << strUrl << endl;

	return strUrl;
}
