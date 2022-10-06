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
#include "bwfsasyncwatcher.h"

BWFSAsyncWatcher::BWFSAsyncWatcher(const BWString &dir) {
    m_dir = dir;
	
	m_cur_buffer = NULL;
	m_compport = NULL;
	
	m_dirinfo = new DIRECTORY_INFO();
}

BWFSAsyncWatcher::~BWFSAsyncWatcher() {
	delete m_dirinfo;
}

void BWFSAsyncWatcher::Start() {
    init();
}

void BWFSAsyncWatcher::Stop() {
    m_log(BWLog::MSG_XDEBUG) << "Parando Async Watcher" << bwlog::endl;

    PostQueuedCompletionStatus( m_compport, 0, 0, NULL );
	
	CloseHandle(m_dirinfo->hDir);
	CloseHandle(m_compport);
}

BWKeyPair BWFSAsyncWatcher::GetNotification() {
	BWKeyPair notif;
    BWKeyPair tmp;
	
	m_log(BWLog::MSG_XDEBUG) << "GetNotification" << bwlog::endl;
	
	notif = getNext();
	
	if (notif["action"] == "O") {
        tmp = getNext();
		
	    notif["action"] = "R";
	    notif["oldname"] = notif["name"];
	    notif["name"] = tmp["name"];
    }

	return notif;
}

BWKeyPair BWFSAsyncWatcher::getNext() {
    BWKeyPair notif;
	
	m_log(BWLog::MSG_XDEBUG) << "getNext" << bwlog::endl;
	
	if (!m_cur_buffer) {
		if (!waitNotification()) {
			return notif;
		}
	}
		
	notif["action"] = getAction(m_cur_buffer->Action);
	notif["name"] = getFileName(m_cur_buffer->FileName, 
	                            m_cur_buffer->FileNameLength);
								
	m_log(BWLog::MSG_XDEBUG) << "NextEntryOffset: " << (int) m_cur_buffer->NextEntryOffset << bwlog::endl;
	
	if (m_cur_buffer->NextEntryOffset) {
	    m_cur_buffer = (PFILE_NOTIFY_INFORMATION) (((LPBYTE)m_cur_buffer) + m_cur_buffer->NextEntryOffset);
	}
	else {
	    m_cur_buffer = NULL;
	}
	
	return notif;
}

bool BWFSAsyncWatcher::waitNotification() {
    BWString msg;
	int rc = 0;
	DWORD numBytes;
	LPDIRECTORY_INFO di = NULL;
	LPOVERLAPPED lpOverlapped = NULL;
	
	m_log(BWLog::MSG_XDEBUG) << "waitNotification" << bwlog::endl;
	
	//memset(m_dirinfo->lpBuffer, 0, MAX_BUFFER);
	
    GetQueuedCompletionStatus(m_compport, &numBytes, (LPDWORD) &di,
                             &lpOverlapped, INFINITE);
							 
	m_log(BWLog::MSG_XDEBUG) << "Passou do GetQueuedCompletionStatus" << bwlog::endl;

    if (di) {
		m_log(BWLog::MSG_XDEBUG) << "Recebeu algo..." << bwlog::endl;
        //m_cur_buffer = (PFILE_NOTIFY_INFORMATION)di->lpBuffer;
		memcpy(m_buffer, di->lpBuffer, MAX_BUFFER);
		m_cur_buffer = (PFILE_NOTIFY_INFORMATION) m_buffer;
	
		ReadDirectoryChangesW(di->hDir,
                di->lpBuffer,
				MAX_BUFFER,
                TRUE,
                FILE_NOTIFY_CHANGE_CREATION |
                FILE_NOTIFY_CHANGE_LAST_WRITE |
                FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_FILE_NAME,
                &di->dwBufLength,
                &di->Overlapped,
                NULL);
		
		m_log(BWLog::MSG_XDEBUG) << "Retornando verdadeiro" << bwlog::endl;
				
		return true;
    }
	
	m_log(BWLog::MSG_XDEBUG) << "Retornando falso" << bwlog::endl;
	
	return false;
}

void BWFSAsyncWatcher::init() {
    BWString msg;
	
	m_log(BWLog::MSG_XDEBUG) << "Metodo init: " << m_dir << bwlog::endl;
	
	memset(m_dirinfo, 0, sizeof(DIRECTORY_INFO));
	memset(m_buffer, 0, MAX_BUFFER);

    m_dirinfo->hDir = CreateFile(m_dir.c_str(),
                FILE_LIST_DIRECTORY,
                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                NULL,
                OPEN_EXISTING,
                FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
                NULL);

    if (m_dirinfo->hDir == INVALID_HANDLE_VALUE) {
	    msg  = "Erro ao criar handler para diretorio [";
		msg += m_dir;
		msg += "]. ";
		msg.AppendInt(GetLastError());
        throw BWError(WIN_ERROR, msg);
    }
	
	m_log(BWLog::MSG_XDEBUG) << "Criando porta" << bwlog::endl;

    m_compport = CreateIoCompletionPort(m_dirinfo->hDir, m_compport, 
                                       (DWORD) m_dirinfo, 0);
									   
	m_log(BWLog::MSG_XDEBUG) << "ReadDirectoryChanges" << bwlog::endl;

	ReadDirectoryChangesW(m_dirinfo->hDir,
                m_dirinfo->lpBuffer,
				MAX_BUFFER,
                TRUE,
                FILE_NOTIFY_CHANGE_CREATION |
                FILE_NOTIFY_CHANGE_LAST_WRITE |
                FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_FILE_NAME,
                &m_dirinfo->dwBufLength,
                &m_dirinfo->Overlapped,
                NULL);
}

BWString BWFSAsyncWatcher::getAction(DWORD act) {
    BWString action;
	
	switch (act) {
        case FILE_ACTION_ADDED: {
            action = "C";
            break;
		}
        case FILE_ACTION_REMOVED: {
            action = "D";
            break;
        }
        case FILE_ACTION_MODIFIED: {
            action = "M";
            break;
        }
        case FILE_ACTION_RENAMED_OLD_NAME: {
            action = "O";
            break;
        }
        case FILE_ACTION_RENAMED_NEW_NAME: {
            action = "N";
            break;
        }
		default: {
            action = "U";
			break;
        }
    }
	
	return action;
}

BWString BWFSAsyncWatcher::getFileName(WCHAR *filename, DWORD len) {
    BWString name;
	char tmp[MAX_BUFFER];
	
	memset(tmp, 0, MAX_BUFFER);
	
	WideCharToMultiByte(CP_UTF8, 0, filename, len / 2, tmp, MAX_BUFFER, NULL, NULL);
	
	name = m_dir;
    name += tmp;
	
	return name;
}

HANDLE BWFSAsyncWatcher::GetHandler() {
    return m_dirinfo->hDir;
}
