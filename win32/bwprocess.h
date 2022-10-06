#ifndef BWPROCESS_H
#define BWPROCESS_H

#include <bwwindefs.h>
#include <psapi.h>
#include <bwstring.h>
#include <bwwinutils.h>
#include <io.h>

class BWProcess {
	public:
		BWProcess();
		BWProcess(DWORD pid);
		BWProcess(const BWString &path);
		~BWProcess();
		
		void SetPid(DWORD pid);
		void SetPath(const BWString &);
		DWORD GetPid();
		BWString GetProcessName();
		BWString GetProcessPath();
		void ExecProgram(const BWString &params = "");
		int WaitProcessEnd();
		
	private:
		HANDLE m_handler;
		DWORD m_pid;
		BWString m_path;
};

#endif // BWPROCESS_H