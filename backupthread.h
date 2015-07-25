#ifndef BACKUPTHREAD_H
#define BACKUPTHREAD_H

#include <QThread>
#include "backupcore.h"

class BackupThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        int exitCode = BackupCore::backup();
        emit resultReady(exitCode);
    }
signals:
    void resultReady(int resultCode);
};

#endif // BACKUPTHREAD_H
