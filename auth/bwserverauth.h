#ifndef BWSERVERAUTH_H
#define BWSERVERAUTH_H

#include <bwstring.h>
#include <bwacceptedserver.h>
#include <bwsocket.h>
#include <bwserver.h>
#include <bwauthorize.h>
#include <bwconfig.h>
#include <bwtemplate.h>
#include <bwlogger.h>
#include <queue>

class BWServerAuth {
	public:
		BWServerAuth(const BWString &cfg);
		~BWServerAuth(void);
		void WaitConn(void);

	private:
		void startUp();
		void loadCfg(const BWString &cfgpath);
		void loadSql(void);

		BWConfig m_cfg;
		BWTemplate *m_sqlUser;
		BWTemplate *m_sqlQryCache;
		BWTemplate *m_sqlQuery;
		BWServer *m_server;
		BWLogger *m_logger;
		queue<BWDbRegistry> m_queue;
};

#endif
