#ifndef BACKUPCORE_H
#define BACKUPCORE_H

#include <QString>
#include <QProcess>
#include <QDebug>
#ifdef Q_OS_WIN
#include "windows.h"
#endif
#include "backupsettings.h"

class BackupCore {
public:
    static int backup();
    static std::string generateCmd(const std::string &cmd);
private:
    static std::string getCurrentTime(const char * = "%Y%m%d%H%M%S");
    static void resolveCmd(std::string & cmd);
};

#endif // BACKUPCORE_H
