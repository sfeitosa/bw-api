#include "bwwindow.h"

BWWindow::BWWindow() {
	m_handler = NULL;
}

BWWindow::BWWindow(HWND handle) {
	SetWindowHandler(handle);
}

BWWindow::~BWWindow() {
}
		
void BWWindow::SetWindowHandler(HWND handle) {
	char text[1024];

	m_handler = handle;

	if (GetWindowText(m_handler, text, 1024) == 0) {
		m_title = "";
	}
	else {
		m_title = BWWinUtils::ConvertToUtf8(text);
	}

	GetWindowThreadProcessId(m_handler, &m_pid);

	m_proc.SetPid(m_pid);
}

HWND BWWindow::GetWindowHandler() {
	return m_handler;
}

BWString BWWindow::GetWindowTitle() {
	return m_title;
}

int BWWindow::GetWindowId() {
	if (!m_handler) {
		return -1;
	}
	
	return (int) m_handler;
}

DWORD BWWindow::GetProcessId() {
	return m_pid;
}

BWProcess BWWindow::GetOwner() {
	return m_proc;
}

