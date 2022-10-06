#include "bwprocess.h"

BWProcess::BWProcess() {
}

BWProcess::BWProcess(DWORD pid) {
	SetPid(pid);
}

BWProcess::BWProcess(const BWString &path) {
	SetPath(path);
}

BWProcess::~BWProcess() {
}

void BWProcess::SetPid(DWORD pid) {
	m_pid = pid;
	
	m_handler = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, m_pid);
}

void BWProcess::SetPath(const BWString &path) {
	m_path = path;
}

DWORD BWProcess::GetPid() {
	return m_pid;
}

BWString BWProcess::GetProcessName() {
	BWString path;
	int pos = 0;
	
	path = GetProcessPath();
	
	pos = path.rfind("\\");
	
	if (pos != BWString::npos) {
		return path.substr(pos + 1);
	}
	
	return path;
}

BWString BWProcess::GetProcessPath() {
	DWORD res;
	char name[1024 * 32];
	BWString resp;
	
	if (m_handler == NULL) {
		return "Unknown";
	}
	
	res = GetProcessImageFileName(m_handler,
								name,
								1024 * 32);
	
	if (res == 0) {
		return "Unknown";
	}
	
	resp = BWWinUtils::ConvertToUtf8(name);
	
	return resp;
}

void BWProcess::ExecProgram(const BWString &params) {
	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;
	BWString cmd = m_path + " " + params; 
	char p[4096];

	if (m_path == "") {
		throw BWError(APP_ERROR, "ExecProgram: Path nao informado");
	}

	if (access(m_path.c_str(), 0) != 0) {
		throw BWError(APP_ERROR, "ExecProgram: Programa informado nao existe.");
	}
	
	memset(&sinfo, 0, sizeof(sinfo));
	memset(&pinfo, 0, sizeof(pinfo));
	memset(p, 0, 1024);
	
	strncpy(p, cmd.c_str(), 4095);
	
	sinfo.cb = sizeof(sinfo);
	
	if (! CreateProcess(NULL, p, 0, 0, false,
		DETACHED_PROCESS, 0, 0, &sinfo, &pinfo)) {
		throw BWError(APP_ERROR, "ExecProgram: Erro ao iniciar processo.");
	}
	
	m_handler = pinfo.hProcess;
	m_pid = pinfo.dwProcessId;
	
	CloseHandle(pinfo.hThread);
}

int BWProcess::WaitProcessEnd() {
	DWORD retcode;
	HANDLE h;
	
	h = OpenProcess(SYNCHRONIZE, FALSE, m_pid);
	
	// Wait until the process finishes
	WaitForSingleObject(h, INFINITE);
	
	if (! GetExitCodeProcess(m_handler, &retcode)) {
		throw BWError(APP_ERROR, 
				"WaitProcessEnd: Erro ao obter codigo de finalizacao");
	}
	
	CloseHandle(h);
	
	return retcode;
}
