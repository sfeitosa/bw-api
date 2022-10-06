//
// C++ Interface: bwnetutils
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __BWNETUTILS_H
#define __BWNETUTILS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <bwstring.h>
#include <bwkeypair.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <fstream>

using std::fstream;

bool iptables_addrule(BWString table, BWString chain, BWString options,
		BWString target, BWString targetopt);
BWKeyPair getgateway(void);
BWKeyPair getifaceipaddr(void);
BWKeyPair getifacemacaddr(void);
BWString getmacaddr(const BWString &iface);
BWKeyPair getifacenetmask(void);
int getabbrevmask(const BWString &mask);
int getBitsFromOctet(int octet);

#endif

