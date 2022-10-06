#ifndef BWPROCESSLIST_H
#define BWPROCESSLIST_H

#include <bwwindefs.h>
#include <psapi.h>
#include <bwstring.h>
#include <bwkeypair.h>
#include <bwprocess.h>

class BWProcessList: public BWKeyPair {
	public:
		BWProcessList();
		~BWProcessList();

		bool FillWithProcess();

		BWProcessList GetAddedProcess(BWProcessList);
		BWProcessList GetRemovedProcess(BWProcessList);
};
	
#endif // BWPROCESSLIST_H