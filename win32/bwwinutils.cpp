#include "bwwinutils.h"

BWWinUtils::BWWinUtils(void) {
}

BWWinUtils::~BWWinUtils(void) {
}

BWString BWWinUtils::GetDosDevice(const BWString &dev) {
	char tmp[1024];
	BWString letter;

	for (char c = 'A'; c < 'Z'; c++) {
		letter.assign(1, c);
		letter += ":";
		
		if (QueryDosDevice(letter.c_str(), tmp, 1024) != 0) {
			if (dev == tmp) {
				return letter;
			}
		}
	}
	
	return "";
}

BWString BWWinUtils::ConvertToUtf8(const BWString &src) {
	BWString dest;
	WCHAR tmp[1024 * 32];
	char output[1024 * 32];
	
	memset(tmp, 0, 1024 * 32 * sizeof(WCHAR));
	memset(output, 0, 1024 * 32 * sizeof(char));

	MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, tmp, 1024 * 32);

	WideCharToMultiByte(CP_UTF8, 0, tmp, -1, output, 1024 * 32, NULL, NULL);

	dest = output;

	return dest;
}

BWString BWWinUtils::GetFileType(const BWString &fname) {
    BWString type;
	DWORD attr;
	
	attr = GetFileAttributes(fname.c_str());
	
	if (attr != 0xffffffff && (attr & FILE_ATTRIBUTE_DIRECTORY)) {
        return "D";
	}
	
    return "F";
}

BWString BWWinUtils::GetPCName() {
    char name[4096];
	DWORD len = 4095;
	
    memset(name, 0, 4096);
	
    GetComputerName(name, &len);
	
    return name;
}

int BWWinUtils::GetIdleTime() {
    LASTINPUTINFO lif;
    DWORD tickCount = 0;
    int idletime = 0;
	
    lif.cbSize = sizeof(LASTINPUTINFO);
	
    GetLastInputInfo(&lif);
	
    tickCount = GetTickCount();
	
    idletime = (tickCount - lif.dwTime) / 1000;
	
    return idletime;
}

BWString BWWinUtils::GetGateway() {
    PIP_ADAPTER_INFO info;
    PIP_ADAPTER_INFO pInfo = NULL;
    DWORD ret = 0;
    ULONG size = sizeof (IP_ADAPTER_INFO);
	BWString gw = "";
	
	info = new IP_ADAPTER_INFO;
	
    if (info == NULL) {
	    throw BWError(WIN_ERROR, "Erro alocando memoria para GetAdapterInfo");
    }
	
	// Chama a funcao para saber o tamanho do buffer necessario
    if (GetAdaptersInfo(info, &size) == ERROR_BUFFER_OVERFLOW) {
		delete info;
		
        info = (IP_ADAPTER_INFO *) new char[size];
		
        if (info == NULL) {
		    throw BWError(WIN_ERROR, 
			         "Erro ao alocar memoria necessaria para GetAdaptersInfo");
        }
    }

	// Chama novamenta a funcao para preencher com os dados
    if ((ret = GetAdaptersInfo(info, &size)) == NO_ERROR) {
	    pInfo = info;

        while (pInfo) {
		    gw = pInfo->GatewayList.IpAddress.String;
			
			if (gw != "") {
			    delete info;
			    return gw;
			}
            
            pInfo = pInfo->Next;
        }
    } 
	
	delete info;
	return gw;
}

void BWWinUtils::WriteRegStr(HKEY key, const BWString &subkey, 
                 const BWString &vname, const BWString &vdata) {
    DWORD ret;
    HKEY openkey;

    ret = RegOpenKeyEx(key, subkey.c_str(), 0, KEY_WRITE, &openkey);
	
    if (ret != ERROR_SUCCESS) {
        ret = RegCreateKeyEx(key, subkey.c_str(), 0, NULL, 
                 REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
                 &openkey, NULL);				 
    }

    RegSetValueEx(openkey, vname.c_str(), 0, REG_SZ, 
	               reinterpret_cast<BYTE *>(const_cast<char *>(vdata.c_str())), 
                   vdata.length());
	
    RegCloseKey(openkey);
}

void BWWinUtils::WriteRegWord(HKEY key, const BWString &subkey,
                 const BWString &vname, DWORD vdata) {
    DWORD ret;
    HKEY openkey;

    ret = RegOpenKeyEx(key, subkey.c_str(), 0, KEY_WRITE, &openkey);
	
    if (ret != ERROR_SUCCESS) {
        ret = RegCreateKeyEx(key, subkey.c_str(), 0, NULL, 
                 REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
                 &openkey, NULL);
    }

    RegSetValueEx(openkey, vname.c_str(), 0, REG_DWORD, 
	              reinterpret_cast<BYTE *>(&vdata), sizeof(vdata));
	
    RegCloseKey(openkey);
}
