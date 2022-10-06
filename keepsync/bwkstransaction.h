#ifndef BWKSTRANSACTION_H
#define BWKSTRANSACTION_H

#include <bwclient.h>
#include <bwconfig.h>
#include <bwlog.h>

class BWKSTransaction {
    public:
        BWKSTransaction(BWConfig &);
        ~BWKSTransaction();

        bool DoConnect();
        void Disconnect();
        BWString HasChanges(const BWString &, const BWString &);
        bool BeginChanges(const BWString &, const BWString &, const BWString &);
        void EndChanges(const BWString &, const BWString &);
        bool GetNextChange(BWKeyPair *);
        bool BeginDeletes(const BWString &, const BWString &, const BWString &);
        void EndDeletes(const BWString &, const BWString &);
        bool GetNextDelete(BWKeyPair *);
        bool BeginLogs(const BWString &);
        void EndLogs(const BWString &);
        BWKeyPair Chat(BWKeyPair);
        void SendLog(BWKeyPair);
        BWString RegisterWorkstation(const BWString &);
		BWString GetDateTime();

    private:

        BWClient *m_client;
        BWConfig m_cfg;
        BWLog m_log;
};

#endif // BWKSTRANSACTION_H

