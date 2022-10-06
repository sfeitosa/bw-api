#ifndef BWKSLOGGER_H
#define BWKSLOGGER_H

#include <bwconfig.h>
#include <bwkstransaction.h>
#include <bwdatabase.h>
#include <bwdb.h>
#include <bwthread.h>
#include <bwutils.h>
#include <bwlist.h>
#include <bwlog.h>
#include <map>

class BWKSLogger: public BWThread {
    public:
        BWKSLogger(BWConfig &);
        ~BWKSLogger();

        void SyncLogs();

    private:
        void run();
        void init();
        void loadDbs();
        void syncSection(const BWString &);
        void syncTables(const BWString &, const BWString &);
        bool syncRegister(BWKeyPair);
        void cleanLogs(const BWString &);
        BWString getTranslatedId(const BWString &, const BWString &,
                                 const BWString &);
        void writeTranslatedId(const BWString &, const BWString &,
                               const BWString &, const BWString &);
        void writeReplicatedData(const BWString &, const BWString &,
                                const BWString &, const BWString &,
                                const BWString &);
        BWKeyPair getRelationsId(BWKeyPair);
        void beginTransaction(const BWString &);
        void commitTransaction(const BWString &);
        void rollbackTransaction(const BWString &);
        // void changeReference(const BWString &, const BWString &, 
        //                    const BWString &, bool);

        BWConfig m_cfg;
        BWKSTransaction *m_transaction;
        BWDb *m_ksdb;
        BWDb *m_appdb; 
        map<BWString,BWDb*> m_dbs;
        BWLog m_log;
};

#endif // BWKSLOGGER_H
