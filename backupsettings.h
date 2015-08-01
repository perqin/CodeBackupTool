#ifndef BACKUPSETTINGS_H
#define BACKUPSETTINGS_H

#include <vector>
#include <string>
#include <utility>
#include <ctime>
#include <map>
#include <functional>
#include <QFileInfo>
#include <QDebug>
#include <fstream>

class BackupSettings {
public:
    typedef std::pair<std::string, std::string> t_par;
    typedef std::vector<t_par> vec;
    typedef std::map<std::string, std::string> t_map;
    static std::string getValue(const std::string & key);
    static void setValue(const std::string & key, const std::string & val);
    static void readSettings();
    static void writeSettings();
private:
    static t_par lineToPair(const std::string & line);
    static std::string pairToLine(const t_par & par);
    static void setDefaultSettings();
    static t_map backupSettings;
};

#endif // BACKUPSETTINGS_H
