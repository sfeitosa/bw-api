#ifndef BWAUTHHANDLER_H
#define BWAUTHHANDLER_H

#include <bwstring.h>
#include <bwdb.h>
#include <bwdatabase.h>
#include <bwacceptedserver.h>
#include <bwthread.h>
#include <bwauthorize.h>
#include <bwkeypair.h>
#include <bwtemplate.h>

class BWAuthHandler: public BWThread {
	public:
		BWAuthHandler(BWAcceptedServer *conn,
			BWConfig cfg, BWTemplate *sqluser, 
			BWTemplate *sqlqrycache, BWTemplate *sqlquery, 
			BWTemplate *sqllogger);
	private:
		void startUp(void);
		void run(void);
		
		BWAcceptedServer *m_conn;
		BWDb *m_db;
		BWTemplate *m_sqlUser;
		BWTemplate *m_sqlQryCache;
		BWTemplate *m_sqlQuery;
		BWTemplate *m_sqlLogger;
		BWConfig m_cfg;
};

#endif // BWAUTHHANDLER_H

