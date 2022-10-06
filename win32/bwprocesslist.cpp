#include "bwprocesslist.h"

BWProcessList::BWProcessList() :
	BWKeyPair() {
}

BWProcessList::~BWProcessList() {
}

bool BWProcessList::FillWithProcess() {
	BWProcess proc;
	DWORD processes[1024];
	DWORD readbytes;
	int proccount;
	BWString key;
	BWString pair;
	
	if (!EnumProcesses(processes, sizeof(processes), &readbytes)) {
		return false;
	}

	clear();
	
	proccount = readbytes / sizeof(DWORD);
	
	for (int i = 0; i < proccount; i++) {
		if (processes[i] != 0) {
			proc.SetPid(processes[i]);
		
			key = BWString("").AppendInt(processes[i]);
			pair = proc.GetProcessName();
			
			operator[](key) = pair;
			
		}
	}
	
	return true;
}

BWProcessList BWProcessList::GetAddedProcess(BWProcessList old) {
	BWProcessList added;
	BWProcessList::iterator itr;
	
	for (itr = begin(); itr != end(); itr++) {
		if (old[itr->first] != itr->second) {			
			added[itr->first] = itr->second;
		}
	}
	
	return added;
}

BWProcessList BWProcessList::GetRemovedProcess(BWProcessList old) {
	BWProcessList removed;
	BWProcessList::iterator itr;

	for (itr = old.begin(); itr != old.end(); itr++) {
		if (GetString(itr->first) != itr->second) {
			removed[itr->first] = itr->second;
		}
	}
	
	return removed;
}
