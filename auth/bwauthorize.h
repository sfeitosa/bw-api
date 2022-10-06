#ifndef BWAUTHORIZE_H
#define BWAUTHORIZE_H

#include <bwacceptedserver.h>
#include <bwdb.h>
#include <bwdatabase.h>
#include <bwstring.h>
#include <bwresultset.h>
#include <bwthread.h>
#include <bwtemplate.h>
#include <bwconfig.h>
#include <bwdbregistry.h>
#include <queue>

class BWAuthorize: public BWThread {
	public:
		BWAuthorize(BWAcceptedServer *conn, BWConfig cfg, 
			BWTemplate *sqlUser, BWTemplate *sqlQryCache, 
			BWTemplate *sqlQuery, queue<BWDbRegistry> *q);

		~BWAuthorize(void);

	private:
		void startUp(void);
		void run(void);
		BWKeyPair authorize(BWKeyPair &);
		BWResultSet doAuth(BWTemplate *sql, const BWString &user_id, BWKeyPair &inputData);
		void updateCache(BWKeyPair &data);
		void logger(BWKeyPair &data);
		BWString getUser(BWKeyPair &kp);
		BWString makeQuery(BWTemplate *qry, const BWString &user_id, BWKeyPair &kp);
		BWString formatReturn(BWResultSet rset, const BWString &user, const BWString &fromcache);
		BWString formatReturn(const BWString &user);

		BWAcceptedServer *m_conn;
		BWDb *m_db;
		BWDb *m_dblogger;
		BWTemplate *m_sqlUser;
		BWTemplate *m_sqlQryCache;
		BWTemplate *m_sqlQuery;
		BWConfig m_cfg;
		BWKeyPair m_inputData;
		queue<BWDbRegistry> *m_queue;
		
		int m_pidc;
};

#endif
