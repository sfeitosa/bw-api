#include "bwprinterjob.h"
/*
BWPrinterJob::BWPrinterJob() {
	m_jobinfo = NULL;
}

BWPrinterJob::BWPrinterJob(BWPrinter printer, DWORD jobid) {
	m_jobinfo = NULL;

	SetJobInfo(printer, jobid);
}

BWPrinterJob::~BWPrinterJob() {
	char *ptr = m_jobinfo;

	if (m_jobinfo) {
		delete[] ptr;
	}
}

void BWPrinterJob::SetJobInfo(BWPrinter printer, DWORD jobid) {
	DWORD bytes;
	DWORD nbytes;
	
	m_printer = printer;
	m_jobid = jobid;
	
	if (m_printer.GetHandler() == NULL) {
		BWError(WIN_ERROR, "Ponteiro para handler de impressora nulo");
	}
	
	bytes = allocJobInfo();
	
	GetJob(m_printer.GetHandler(), m_jobid, 2, (LPBYTE) m_jobinfo, bytes, &nbytes);
}

BWString BWPrinterJob::GetPrinterName() {
	if (!m_jobinfo) {
		throw BWError(WIN_ERROR, "Ponteiro para JOB_INFO nulo");
	}

	return m_jobinfo->pPrinterName;
}

BWString BWPrinterJob::GetMachineName() {
	if (!m_jobinfo) {
		throw BWError(WIN_ERROR, "Ponteiro para JOB_INFO nulo");
	}
	
	return m_jobinfo->pMachineName;
}

BWString BWPrinterJob::GetUserName() {
	if (!m_jobinfo) {
		throw BWError(WIN_ERROR, "Ponteiro para JOB_INFO nulo");
	}
	
	return m_jobinfo->pUserName;
}

BWString BWPrinterJob::GetDocName() {
	if (!m_jobinfo) {
		throw BWError(WIN_ERROR, "Ponteiro para JOB_INFO nulo");
	}
	
	return m_jobinfo->pDocument;
}

BWString BWPrinterJob::GetDriverName() {
	if (!m_jobinfo) {
		throw BWError(WIN_ERROR, "Ponteiro para JOB_INFO nulo");
	}
	
	return m_jobinfo->pDriverName;
}

DWORD BWPrinterJob::GetStatus() {
	if (!m_jobinfo) {
		throw BWError(WIN_ERROR, "Ponteiro para JOB_INFO nulo");
	}
	
	return m_jobinfo->Status;
}

DWORD BWPrinterJob::GetTotalPages() {
	if (!m_jobinfo) {
		throw BWError(WIN_ERROR, "Ponteiro para JOB_INFO nulo");
	}
	
	return m_jobinfo->TotalPages;
}

DWORD BWPrinterJob::GetTotalSize() {
	if (!m_jobinfo) {
		throw BWError(WIN_ERROR, "Ponteiro para JOB_INFO nulo");
	}
	
	return m_jobinfo->Size;
}

DWORD BWPrinterJob::GetPagesPrinted() {
	if (!m_jobinfo) {
		throw BWError(WIN_ERROR, "Ponteiro para JOB_INFO nulo");
	}
	
	return m_jobinfo->PagesPrinted;
}

DWORD BWPrinterJob::allocJobInfo() {
	DWORD bytes;

	GetJob(m_printer.GetHandler(), m_jobid, 2, (LPBYTE) m_jobinfo, NULL, &bytes);
	
	m_jobinfo = reinterpret_cast<JOB_INFO_2 *>(new char[bytes]);
	
	return bytes;
}

void BWPrinterJob::Pause() {
	if (!SetJob(m_printer.GetHandler(), m_jobid, 
				0, NULL, JOB_CONTROL_PAUSE)) 
	{
		throw BWError(WIN_ERROR, "Erro ao pausar job");
	}
}

void BWPrinterJob::Resume() {
	if (!SetJob(m_printer.GetHandler(), m_jobid, 
				0, NULL, JOB_CONTROL_RESUME)) 
	{
		throw BWError(WIN_ERROR, "Erro ao resumir job");
	}
}

void BWPrinterJob::Delete() {
	if (!SetJob(m_printer.GetHandler(), m_jobid, 
				0, NULL, JOB_CONTROL_DELETE)) 
	{
		throw BWError(WIN_ERROR, "Erro ao deletar job");
	}
}

void BWPrinterJob::Restart() {
	if (!SetJob(m_printer.GetHandler(), m_jobid, 
				0, NULL, JOB_CONTROL_RESTART)) 
	{
		throw BWError(WIN_ERROR, "Erro ao reiniciar job");
	}
}
*/