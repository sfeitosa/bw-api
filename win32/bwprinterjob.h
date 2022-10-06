#ifndef BWPRINTERJOB_H
#define BWPRINTERJOB_H
/*
#include <windows.h>
#include <bwprinter.h>

class BWPrinterJob {
	public:
		BWPrinterJob();
		BWPrinterJob(BWPrinter, DWORD);
		~BWPrinterJob();
		
		void SetJobInfo(BWPrinter, DWORD);
		
		BWString GetPrinterName();
		BWString GetMachineName();
		BWString GetUserName();
		BWString GetDocName();
		BWString GetDriverName();
		DWORD GetStatus();
		DWORD GetTotalPages();
		DWORD GetTotalSize();
		DWORD GetPagesPrinted();
		
		void Pause();
		void Resume();
		void Delete();
		void Restart();
		
	private:
		DWORD allocJobInfo();
	
		BWPrinter m_printer;
		DWORD m_jobid;
		JOB_INFO_2 *m_jobinfo;
};
*/
#endif // BWPRINTERJOB_H
