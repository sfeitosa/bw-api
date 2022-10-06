//
// C++ Implementation: bwnetutils
//
// Description:
//
//
// Author:  <samuel@beanweb.com.br>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <bwnetutils.h>

bool iptables_addrule(BWString table, BWString chain, BWString options,
		BWString target, BWString targetopt) {
	BWString rule;

	rule = "/sbin/iptables -t " + table + " -A " + chain;

	if (options.length() > 0 && options != "(null)") {
		rule += " " + options;
	}

	rule += " -j " + target;

	if (targetopt.length() > 0 && targetopt != "(null)") {
		rule += " " + targetopt;
	}

        if (table.find(";") == BWString::npos &&
		chain.find(";") == BWString::npos &&
		options.find(";") == BWString::npos &&
		target.find(";") == BWString::npos &&
		targetopt.find(";") == BWString::npos) {
	
		system(rule.c_str());

		return true;
	}
	
	return false;
}

BWKeyPair getgateway(void) {
	BWString line;
	BWList list;
	BWKeyPair kp;
	fstream f("/proc/net/route", fstream::in);
	unsigned long dest, gw;
	int count = 0;
	struct in_addr in_addr;

	while (getline(f, line)) {
		if (count > 0) {
			list = line.Split("\t");
	
			sscanf(list[1].c_str(), "%lx", &dest);
			sscanf(list[2].c_str(), "%lx", &gw);

			if (dest == 0) {
				in_addr.s_addr = gw;
				kp[list[0]] = inet_ntoa(in_addr);
				//break;
			}
		}
		count++;
	}

	return kp;
}

BWKeyPair getifaceipaddr(void) {
	BWKeyPair list;
	struct ifreq ifreqs[20];
	struct sockaddr_in *addr;
        struct in_addr address;
	struct ifconf ifconf;
	int nifaces, i, sock;
	BWString iface;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		throw BWError(SOCK_ERROR, "Erro ao criar socket");
	}

	memset(&ifconf, 0, sizeof(ifconf));
	ifconf.ifc_buf = (char *) (ifreqs);
	ifconf.ifc_len = sizeof(ifreqs);

	if (ioctl(sock, SIOCGIFCONF, (char *) &(ifconf)) < 0) {
		throw BWError(SOCK_ERROR, 
			"Erro ao executar ioctl (SIOCGIFCONF)"); 
	}

	nifaces = ifconf.ifc_len / sizeof (struct ifreq);

	for (i = 0; i < nifaces; i++) {
		iface = ifreqs[i].ifr_name;
		addr = (struct sockaddr_in *) &(ifreqs[i].ifr_addr);
                address = addr->sin_addr;
		
		list[iface] = inet_ntoa(address);
	}

	close(sock);

	return list;
}

BWKeyPair getifacemacaddr(void) {
	BWKeyPair list;
	struct ifreq ifreqs[20];
	struct ifreq *ifr, *ifend;
	struct ifreq ifreq;
	struct ifconf ifconf;
	int sock;
	BWString iface;
	char hw[30];

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		throw BWError(SOCK_ERROR, "Erro ao criar socket");
	}

	memset(&ifconf, 0, sizeof(ifconf));
	ifconf.ifc_buf = (char *) (ifreqs);
	ifconf.ifc_len = sizeof(ifreqs);

	if (ioctl(sock, SIOCGIFCONF, (char *) &(ifconf)) < 0) {
		throw BWError(SOCK_ERROR, 
			"Erro ao executar ioctl (SIOCGIFCONF)"); 
	}

	ifend = ifreqs + (ifconf.ifc_len / sizeof(struct ifreq));

	for (ifr = ifconf.ifc_req; ifr < ifend; ifr++) {
		if (ifr->ifr_addr.sa_family == AF_INET) {
			strncpy(ifreq.ifr_name, 
				ifr->ifr_name,
				sizeof(ifreq.ifr_name));

			if (ioctl(sock, SIOCGIFHWADDR, &ifreq) < 0) {
				// erro ao executar ioctl
			}

			iface = ifreq.ifr_name;

			sprintf(hw, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", 
				(unsigned char) ifreq.ifr_hwaddr.sa_data[0],
				(unsigned char)	ifreq.ifr_hwaddr.sa_data[1],
				(unsigned char)	ifreq.ifr_hwaddr.sa_data[2],
				(unsigned char)	ifreq.ifr_hwaddr.sa_data[3],
				(unsigned char)	ifreq.ifr_hwaddr.sa_data[4],
				(unsigned char)	ifreq.ifr_hwaddr.sa_data[5]);
			list[iface] = hw;
		}
	}

	close(sock);

	return list;
}

BWString getmacaddr(const BWString &iface) {
    BWString macaddr = "";
    char hw[30];
    struct ifreq s;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strcpy(s.ifr_name, iface.c_str());

    if (ioctl(fd, SIOCGIFHWADDR, &s) == 0) {
	sprintf(hw, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", 
				(unsigned char) s.ifr_hwaddr.sa_data[0],
				(unsigned char)	s.ifr_hwaddr.sa_data[1],
				(unsigned char)	s.ifr_hwaddr.sa_data[2],
				(unsigned char)	s.ifr_hwaddr.sa_data[3],
				(unsigned char)	s.ifr_hwaddr.sa_data[4],
				(unsigned char)	s.ifr_hwaddr.sa_data[5]);
        macaddr = hw;
    }

    return macaddr;
}

BWKeyPair getifacenetmask(void) {
	BWKeyPair list;
	struct ifreq ifreqs[20];
	struct ifreq *ifr, *ifend;
	struct ifreq ifreq;
	struct sockaddr_in *addr;
        struct in_addr address;
	struct ifconf ifconf;
	int sock;
	BWString iface;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		throw BWError(SOCK_ERROR, "Erro ao criar socket");
	}

	memset(&ifconf, 0, sizeof(ifconf));
	ifconf.ifc_buf = (char *) (ifreqs);
	ifconf.ifc_len = sizeof(ifreqs);

	if (ioctl(sock, SIOCGIFCONF, (char *) &(ifconf)) < 0) {
		throw BWError(SOCK_ERROR, 
			"Erro ao executar ioctl (SIOCGIFCONF)"); 
	}

	ifend = ifreqs + (ifconf.ifc_len / sizeof(struct ifreq));

	for (ifr = ifconf.ifc_req; ifr < ifend; ifr++) {
		if (ifr->ifr_addr.sa_family == AF_INET) {
			strncpy(ifreq.ifr_name, 
				ifr->ifr_name,
				sizeof(ifreq.ifr_name));

			if (ioctl(sock, SIOCGIFNETMASK, &ifreq) < 0) {
				// erro ao executar ioctl
			}

			iface = ifreq.ifr_name;

			addr = (struct sockaddr_in *) &(ifreq.ifr_netmask);

                        address = addr->sin_addr;

			list[iface] = inet_ntoa(address);
		}
	}

	close(sock);

	return list;
}

int getabbrevmask(const BWString &mask) {
	BWList parts;
	int bits = 0;

	parts = mask.Split(".");

	for (unsigned int i = 0; i < parts.size(); i++) {
		bits += getBitsFromOctet(parts[i].GetInt());
	}

	return bits;
}

int getBitsFromOctet(int octet) {
	switch(octet) {
		case 0: return 0;
		case 128: return 1;
		case 192: return 2;
		case 224: return 3;
		case 240: return 4;
		case 248: return 5;
		case 252: return 6;
		case 254: return 7;
		case 255: return 8;
	}

	return -999;
}
