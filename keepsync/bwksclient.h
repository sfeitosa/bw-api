#ifndef BWKSCLIENT_H
#define BWKSCLIENT_H

#include <bwthread.h>
#include <bwconfig.h>
#include <bwkssynchronizer.h>
#include <bwkslogger.h>
#include <bwutils.h>
#include <bwlog.h>

class BWKSClient: public BWThread {
    public:
        BWKSClient(BWConfig &);
        BWKSClient(const BWString &);
        ~BWKSClient();
		
		bool CheckWorkstation();
		BWString GetDateTime();

    private:
		void init();
        void run();
        void startSync();
        void startLogger();

        BWConfig m_cfg;
        BWKSSynchronizer *m_sync;
        BWKSLogger *m_logger;
        BWLog m_log;
};

#endif // BWKSCLIENT_H

