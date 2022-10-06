#include "bwdrivelist.h"

BWDriveList::BWDriveList() :
	BWKeyPair() {
}

BWDriveList::~BWDriveList() {
}

void BWDriveList::FillWithDrives() {
    char devices[4096];
    int len;
    BWString dev;
	
    len = GetLogicalDriveStrings(4095, devices);
	
	clear();
    
    for (int i = 0; i < len; i++) {
	    if (devices[i] != '\0') {
            dev += devices[i];
        }
		else {
		    operator[](dev) = getDriveType(dev);
            dev = "";
		}
	}
}

BWDriveList BWDriveList::GetAddedDrives(BWDriveList old) {
	BWDriveList added;
	BWDriveList::iterator itr;
	
	for (itr = begin(); itr != end(); itr++) {
		if (old[itr->first] != itr->second) {			
			added[itr->first] = itr->second;
		}
	}
	
	return added;
}

BWDriveList BWDriveList::GetRemovedDrives(BWDriveList old) {
	BWDriveList removed;
	BWDriveList::iterator itr;

	for (itr = old.begin(); itr != old.end(); itr++) {
		if (GetString(itr->first) != itr->second) {
			removed[itr->first] = itr->second;
		}
	}
	
	return removed;
}

BWString BWDriveList::getDriveType(const BWString &dev) {
    unsigned int type;

    type = GetDriveType(dev.c_str());
	
    switch (type) {
        case 0: return "U"; // unknown
        case 1: return "I"; // invalid
        case 2: return "R"; // removable
        case 3: return "F"; // fixed
        case 4: return "N"; // remote
        case 5: return "M"; // cdrom
        case 6: return "D"; // ramdisk
	}
	
	return "";
}
