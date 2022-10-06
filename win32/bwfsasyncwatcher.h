//
// C++ Interface: BWFSAsyncWatcher
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef BWFSASYNCWATCHER_H
#define BWFSASYNCWATCHER_H

//#include <bwwindefs.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include <bwerror.h>
#include <bwkeypair.h>
#include <bwstring.h>
#include <bwlog.h>

#define MAX_BUFFER 32768

typedef struct _DIRECTORY_INFO {
      HANDLE      hDir;
      CHAR        lpBuffer[MAX_BUFFER];
      DWORD       dwBufLength;
      OVERLAPPED  Overlapped;
} DIRECTORY_INFO, *PDIRECTORY_INFO, *LPDIRECTORY_INFO;

class BWFSAsyncWatcher {
    public:
	    BWFSAsyncWatcher(const BWString &dir);
		~BWFSAsyncWatcher();
		
		void Start();
		void Stop();
		BWKeyPair GetNotification();
		HANDLE GetHandler();
		
	private:
	
        void init();
		bool waitNotification();
		BWKeyPair getNext();
		BWString getAction(DWORD act);
		BWString getFileName(WCHAR *filename, DWORD len);
		
		
		//HANDLE m_dir_handler;
		BWString m_dir;
		//FILE_NOTIFY_INFORMATION *m_buffer;
		FILE_NOTIFY_INFORMATION *m_cur_buffer;
		
		PDIRECTORY_INFO m_dirinfo;
		HANDLE m_compport;
		BWLog m_log;
		CHAR m_buffer[MAX_BUFFER];
};

#endif //BWFSASYNCWATCHER_H
