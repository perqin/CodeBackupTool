#ifndef BACKUPCORE_H
#define BACKUPCORE_H

#include <QString>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include <functional>
#include <ctime>
#include <QProcess>
#include <fstream>
#include <QFileInfo>
#include <QDebug>
#ifdef Q_OS_WIN
#include "windows.h"
#endif

class BackupCore
{
public:
    typedef std::pair<std::string, std::string> t_par;
    typedef std::vector<t_par> vec;
    typedef std::map<std::string, std::string> t_map;
    static int backup();
private:
    static t_par lineToPair(const std::string & line);
    static std::string pairToLine(const t_par & par);
    static void readSettings();
    static void setDefaultSettings();
    static void writeSettings();
    static std::string getCurrentTime(const char * = "%Y%m%d%H%M%S");
    static void resolveCmd(std::string & cmd);
    static int currentVersion;
    static t_map backupSettings;
};

#endif // BACKUPCORE_H
