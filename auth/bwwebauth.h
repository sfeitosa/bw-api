#ifndef BWWEBAUTH_H
#define BWWEBAUTH_H

#include <bwstring.h>
#include <bwkeypair.h>
#include <bwconfig.h>
#include <bwclient.h>
#include <bwlist.h>
#include <bwurl.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

class BWWebAuth {
	public:
		BWWebAuth(const BWString &cfg);
		~BWWebAuth(void);
		void Run(void);

	private:
		void loadCfg(const BWString &cfg);
		void startUp(void);
		void doConnect();
		void waitInputData(void);
		BWString formatInput(const BWString &input);
		BWString formatOutput(const BWString &readdata, const BWString &urlIn);

		BWConfig m_cfg;
		BWClient *m_client;
		int m_pid;
};

#endif // BWWEBAUTH_H
