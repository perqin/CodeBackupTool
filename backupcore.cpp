#include "backupcore.h"

int BackupCore::currentVersion = 0;
BackupCore::t_map BackupCore::backupSettings = BackupCore::t_map();

BackupCore::t_par BackupCore::lineToPair(const std::string &line) {
    t_par k_v(std::string(""), std::string(""));
    if (line.size() == 0)
        return k_v;
    std::function<std::string(const std::string &)> parseIt = [](const std::string & s)->std::string {
        if (s.at(0) != '\"')
            return s;
        std::string r;
        for (unsigned i = 1; i < s.size() - 1; ++i) {
            if (s.at(i) == '\"' && i != s.size() - 2 && s.at(i + 1) == '\"')
                ++i;
            r.push_back(s.at(i));
        }
        return r;
    };
    bool inQuote = false;
    std::string::const_iterator si = line.begin();
    while (si != line.end()) {
        if (*si == ':' && !inQuote) {
            k_v.first = parseIt(std::string(line.begin(), si));
            ++si;
            k_v.second = parseIt(std::string(si, line.end()));
            break;
        }
        if (*si == '\"')
            inQuote = !inQuote;
        ++si;
    }
    return k_v;
}

std::string BackupCore::pairToLine(const t_par &par) {
    std::function<std::string(const std::string &)> parseIt = [](const std::string & s)->std::string {
        std::string r;
        r.push_back('\"');
        for (unsigned i = 0; i < s.size(); ++i) {
            if (s.at(i) == '\"')
                r.push_back(s.at(i));
            r.push_back(s.at(i));
        }
        r.push_back('\"');
        return r;
    };
    return parseIt(par.first) + ":" + parseIt(par.second);
}

int BackupCore::backup() {
    readSettings();
    std::string zipCmd;
    resolveCmd(zipCmd);
    qDebug() << zipCmd.c_str();
    Sleep(uint(8000));
    qDebug() << "finish";
//    QProcess process;
//    process.start(zipCmd.c_str());
//    if (!process.waitForStarted())
//        return 1;
//    if (!process.waitForFinished(-1))
//        return 2;
    return 0;
}

std::string BackupCore::getCurrentTime(const char * format) {
    std::time_t rawtime;
    struct std::tm *timeinfo;
    char timeBuffer[80];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(timeBuffer, 80, format, timeinfo);
    return std::string(timeBuffer);
}

void BackupCore::readSettings() {
    setDefaultSettings();
    QFileInfo fileInfo("./config");
    if (fileInfo.exists() && fileInfo.isFile()) {
        std::ifstream fin;
        std::string str;
        t_par p;
        t_map::iterator fi;
        fin.open("./config");
        while (std::getline(fin, str)) {
            if (str.size() > 0 && str.at(0) != '#') {
                p = lineToPair(str);
                fi = backupSettings.find(p.first);
                if (fi == backupSettings.end()) {
                    backupSettings.insert(p);
                } else {
                    fi->second = p.second;
                }
            }
        }
        fin.close();
    } else {
        writeSettings();
    }
}

void BackupCore::writeSettings() {
    std::vector<std::string> outList;
    QFileInfo fileInfo("./config");
    if (fileInfo.exists() && fileInfo.isFile()) {
        std::ifstream fin;
        std::string str;
        t_map::iterator fi;
        t_par p;
        fin.open("./config");
        while (std::getline(fin, str)) {
            if (str.size() == 0 || str.at(0) == '#') {
                outList.push_back(str);
            } else {
                p = lineToPair(str);
                fi = backupSettings.find(p.first);
                if (fi == backupSettings.end()) {
                    outList.push_back(str);
                } else {
                    outList.push_back(pairToLine(*fi));
                }
            }
        }
        fin.close();
    } else {
        for (t_map::iterator i = backupSettings.begin(); i != backupSettings.end(); ++i) {
            outList.push_back(pairToLine(*i));
        }
    }
    std::ofstream fout;
    fout.open("./config");
    for (unsigned i = 0; i < outList.size(); ++i) {
        fout << outList.at(i) << std::endl;
    }
    fout.close();
}

void BackupCore::setDefaultSettings() {
    /*
     * workspace_folder - Workspace folder
     * backup_folder    - Backup zip files folder
     * time_format      - Time format
     * zip_filename     - Zip file name format
     * zip_exe_path     - Zip command line software path
     * zip_cmd          - Zip exec cmd command
     * delete_old       - Whether delete out-of-date backup zip files
     */
    std::vector<t_par> v;
    v.push_back(make_pair(std::string("workspace_folder"),  std::string("C:\\Workspaces")));
    v.push_back(make_pair(std::string("backup_folder"),     std::string("C:\\Workspaces_bak")));
    v.push_back(make_pair(std::string("time_format"),       std::string("%Y%m%d%H%M%S")));
    v.push_back(make_pair(std::string("zip_filename"),      std::string("Workspace_Bak_%TIME%.zip")));
    v.push_back(make_pair(std::string("zip_exe_path"),      std::string("C:\\Program Files\\WinRAR\\Rar.exe")));
    v.push_back(make_pair(std::string("zip_cmd"),           std::string("%EXEC% a \"%BAK_DIR%\\%FILE_NAME%\" \"%SRC_DIR%\"")));
    v.push_back(make_pair(std::string("delete_old"),        std::string("true")));
    t_map::iterator fi;
    for (unsigned i = 0; i < v.size(); ++i) {
        fi = backupSettings.find(v.at(i).first);
        if (fi == backupSettings.end()) {
            backupSettings.insert(v.at(i));
        } else {
            fi->second = v.at(i).second;
        }
    }
}

void BackupCore::resolveCmd(std::string &cmd) {
    QString v_workspace_folder(backupSettings.find("workspace_folder")->second.c_str());
    QString v_backup_folder(backupSettings.find("backup_folder")->second.c_str());
    QString v_time_format(backupSettings.find("time_format")->second.c_str());
    QString v_zip_filename(backupSettings.find("zip_filename")->second.c_str());
    QString v_zip_exe_path(backupSettings.find("zip_exe_path")->second.c_str());
    QString cm(backupSettings.find("zip_cmd")->second.c_str());
    QString cm_old;
    do {
        cm_old = cm;
        cm.replace("%EXEC%", v_zip_exe_path);
        cm.replace("%BAK_DIR%", v_backup_folder);
        cm.replace("%SRC_DIR%", v_workspace_folder);
        cm.replace("%FILE_NAME%", v_zip_filename);
        cm.replace("%TIME%", QString(getCurrentTime(v_time_format.toStdString().c_str()).c_str()));
    } while (cm_old != cm);
    cmd = cm.toStdString();
}
