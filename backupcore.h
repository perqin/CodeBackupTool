#ifndef BACKUPCORE_H
#define BACKUPCORE_H

#include <QString>
#include <vector>
#include <string>
#include <utility>
#include <ctime>
#include <QProcess>
#include <fstream>
#include <QFileInfo>
#ifdef Q_OS_WIN
#include "windows.h"
#endif

class BackupCore
{
public:
    typedef std::pair<std::string, std::string> par;
    typedef std::vector<par> vec;
    static int backup();
private:
    static vec readSettings();
    static void setDefaultSettings(vec & settings);
    static void writeSettings(const vec & settings);
    static std::string getCurrentTime(const char * = "%Y%m%d%H%M%S");
    static void updateSettings(int oldVersion);
    static void resolveCmd(const vec & settings, std::string & cmd);
    static int currentVersion;
};

#endif // BACKUPCORE_H
