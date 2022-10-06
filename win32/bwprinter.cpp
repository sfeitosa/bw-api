#include "bwprinter.h"
/*
BWPrinter::BWPrinter() {
	m_printer = "";
	m_handler = NULL;
}

BWPrinter::BWPrinter(const BWString &prt) {
	m_printer = prt;
	m_handler = NULL;
	
	Open(prt);
}

BWPrinter::~BWPrinter() {
	if (m_handler) {
		Close();
	}
}
		
void BWPrinter::Open(const BWString &prt) {
	PRINTER_DEFAULTS pd;

	m_printer = prt;
	
	pd.pDatatype = NULL;
	pd.pDevMode = NULL;
	pd.DesiredAccess = PRINTER_ALL_ACCESS;
	
	if (!OpenPrinter(m_printer.c_str(), &m_handler, &pd)) {
		throw BWError(WIN_ERROR, "Erro ao abrir handler de impressora");
	}
}

void BWPrinter::Close() {
	ClosePrinter(&m_handler);
}

void BWPrinter::Pause() {
	if (!SetPrinter(m_handler, 0, NULL, PRINTER_CONTROL_PAUSE)) {
		throw BWError(WIN_ERROR, "Erro ao pausar impressora");
	}
}

void BWPrinter::Resume() {
	if (!SetPrinter(m_handler, 0, NULL, PRINTER_CONTROL_RESUME)) {
		throw BWError(WIN_ERROR, "Erro ao continuar impressao");
	}
}

void BWPrinter::PurgeJobs() {
	if (!SetPrinter(m_handler, 0, NULL, PRINTER_CONTROL_PURGE)) {
		throw BWError(WIN_ERROR, 
				"Erro ao cancelar todos os trabalhos de impressao");
	}
}

void BWPrinter::DeleteJob(int) {
}

HANDLE BWPrinter::GetHandler() {
	return m_handler;
}
*/