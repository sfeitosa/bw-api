#ifndef BWWINDOW_H
#define BWWINDOW_H

#include <bwwindefs.h>
#include <bwstring.h>
#include <bwprocess.h>
#include <bwwinutils.h>

class BWWindow {
	public:
		BWWindow();
		BWWindow(HWND);
		~BWWindow();
		
		void SetWindowHandler(HWND);
		HWND GetWindowHandler();
		BWString GetWindowTitle();
		int GetWindowId();
		DWORD GetProcessId();
		BWProcess GetOwner();
		
	private:
		HWND m_handler;	
		DWORD m_pid;
		BWProcess m_proc;
		BWString m_title;
};

#endif // BWWINDOW_H
