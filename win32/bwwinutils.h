#ifndef BWWINUTILS_H
#define BWWINUTILS_H

#include <bwwindefs.h>
#include <bwstring.h>
#include <winsock2.h>
#include <iphlpapi.h>

class BWWinUtils {
	public:
		BWWinUtils(void);
		~BWWinUtils(void);
		
		static BWString GetDosDevice(const BWString &);
		static BWString GetDosName(const BWString &);
		static BWString ConvertToUtf8(const BWString &);
		static BWString GetFileType(const BWString &);
		static BWString GetPCName();
		static int GetIdleTime();
		static BWString GetGateway();
		static void WriteRegStr(HKEY key, const BWString &subkey, 
		                 const BWString &vname, const BWString &vdata);
		static void WriteRegWord(HKEY key, const BWString &subkey,
		                 const BWString &vname, DWORD vdata);
};

#endif // BWWINUTILS_H
