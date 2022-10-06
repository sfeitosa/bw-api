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
#include "bwfswatcher.h"

BWFSWatcher::BWFSWatcher(const BWString &dir) {
    m_dir = dir;
	m_dir_handler = NULL;
	
	//m_buffer = (FILE_NOTIFY_INFORMATION *) malloc (32 * 1024);
	m_buffer = reinterpret_cast<FILE_NOTIFY_INFORMATION *>(new char[32 * 1024]);
	m_cur_buffer = NULL;
}

BWFSWatcher::~BWFSWatcher() {
    delete[] m_buffer;

    Stop();
}

void BWFSWatcher::Start() {
    init();
}

void BWFSWatcher::Stop() {
    if (m_dir_handler) {
        CloseHandle(m_dir_handler);
    }
}

BWKeyPair BWFSWatcher::GetNotification() {
	BWKeyPair notif;
    BWKeyPair tmp;
	
	notif = getNext();
	
	if (notif["action"] == "O") {
        tmp = getNext();
		
	    notif["action"] = "R";
	    notif["oldname"] = notif["name"];
	    notif["name"] = tmp["name"];
    }

	return notif;
}

BWKeyPair BWFSWatcher::getNext() {
    BWKeyPair notif;
	
	if (!m_cur_buffer) {
		waitNotification();
	}
	
	notif["action"] = getAction(m_cur_buffer->Action);
	notif["name"] = getFileName(m_cur_buffer->FileName, 
	                            m_cur_buffer->FileNameLength);
	
	if (m_cur_buffer->NextEntryOffset) {
	    m_cur_buffer = (FILE_NOTIFY_INFORMATION *) (((BYTE*)m_cur_buffer) + m_cur_buffer->NextEntryOffset);
	}
	else {
	    m_cur_buffer = NULL;
	}
	
	return notif;
}

void BWFSWatcher::waitNotification() {
    BWString msg;
    DWORD nbytes;
	int rc = 0;
	
	memset(m_buffer, 0, 32 * 1024);
	
    rc = ReadDirectoryChangesW(m_dir_handler,
                m_buffer,
				32 * 1024,
                TRUE,
                FILE_NOTIFY_CHANGE_CREATION |
                FILE_NOTIFY_CHANGE_LAST_WRITE |
                FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_FILE_NAME,
                &nbytes,
                NULL,
                NULL);

    if (!rc) {
        msg += "Erro ao iniciar monitoramento de diretorio";
		msg.AppendInt(GetLastError());
		
	    throw BWError(WIN_ERROR, msg);
	}
	
	m_cur_buffer = m_buffer;
}

void BWFSWatcher::init() {
    BWString msg;

    m_dir_handler = CreateFile( m_dir.c_str(),
                FILE_LIST_DIRECTORY,
                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                NULL,
                OPEN_EXISTING,
                FILE_FLAG_BACKUP_SEMANTICS,
                NULL);

    if (m_dir_handler == INVALID_HANDLE_VALUE) {
	    msg  = "Erro ao criar handler para diretorio [";
		msg += m_dir;
		msg += "]. ";
		msg.AppendInt(GetLastError());
        throw BWError(WIN_ERROR, msg);
    }
}

BWString BWFSWatcher::getAction(DWORD act) {
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

BWString BWFSWatcher::getFileName(WCHAR *filename, DWORD len) {
    BWString name;
	char tmp[32 * 1024];
	
	memset(tmp, 0, 32 * 1024);
	
	WideCharToMultiByte(CP_UTF8, 0, filename, len / 2, tmp, 1024 * 32, NULL, NULL);
	
	name = m_dir;
    name += tmp;
	
	return name;
}

HANDLE BWFSWatcher::GetHandler() {
    return m_dir_handler;
}
