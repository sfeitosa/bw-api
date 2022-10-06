#ifndef BWKSSYNCHRONIZER_H
#define BWKSSYNCHRONIZER_H

#include <bwkstransaction.h>
#include <bwconfig.h>
#include <bwdb.h>
#include <bwdatabase.h>
#include <bwthread.h>
//#include <bwwinutils.h>
#include <bwutils.h>
#include <bwlog.h>

class BWKSSynchronizer: public BWThread {
    public:
        BWKSSynchronizer(BWConfig &);
        ~BWKSSynchronizer();

        bool RegisterWorkstation();
        BWString GetDateTime();
        void SyncRules();

    private:
        void run();
        void init();
        void beginTransaction();
        void commitTransaction();
        void rollbackTransaction();
        void syncSection(const BWString &);
        void processDeletes(const BWString &, const BWString &, 
                            const BWString &);
        void processChanges(const BWString &, const BWString &,
                            const BWString &);
        void deleteToDatabase(BWKeyPair);
        void writeToDatabase(BWKeyPair);

        BWConfig m_cfg;
        BWKSTransaction *m_transaction;
        BWDb *m_ksdb;
        BWDb *m_appdb;
        BWLog m_log;
};

#endif // BWKSSYNCHONIZER_H
