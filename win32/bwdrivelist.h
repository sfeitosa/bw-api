#ifndef BWDRIVELIST_H
#define BWDRIVELIST_H

#include <bwwindefs.h>
#include <bwstring.h>
#include <bwkeypair.h>

class BWDriveList: public BWKeyPair {
    public:
        BWDriveList();
        ~BWDriveList();

        void FillWithDrives();

        BWDriveList GetAddedDrives(BWDriveList);
        BWDriveList GetRemovedDrives(BWDriveList);

    private:
        BWString getDriveType(const BWString &);

};
	
#endif // BWDRIVELIST_H
