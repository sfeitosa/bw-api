//
// C++ Interface: BWFSWatcher
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef BWFSWATCHER_H
#define BWFSWATCHER_H

#include <bwwindefs.h>
#include <bwerror.h>
#include <bwkeypair.h>
#include <bwstring.h>

class BWFSWatcher {
    public:
	    BWFSWatcher(const BWString &dir);
		~BWFSWatcher();
		
		void Start();
		void Stop();
		BWKeyPair GetNotification();
		HANDLE GetHandler();
		
	private:
	
        void init();
		void waitNotification();
		BWKeyPair getNext();
		BWString getAction(DWORD act);
		BWString getFileName(WCHAR *filename, DWORD len);
		
		
		HANDLE m_dir_handler;
		BWString m_dir;
		FILE_NOTIFY_INFORMATION *m_buffer;
		FILE_NOTIFY_INFORMATION *m_cur_buffer;
};

#endif //BWFSWATCHER_H
