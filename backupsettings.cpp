#include "backupsettings.h"

BackupSettings::t_map BackupSettings::backupSettings = BackupSettings::t_map();

std::string BackupSettings::getValue(const std::string &key) {
    t_map::iterator fi = backupSettings.find(key);
    if (fi == backupSettings.end())
        return std::string("");
    return fi->second;
}

void BackupSettings::setValue(const std::string &key, const std::string &val) {
    t_map::iterator fi = backupSettings.find(key);
    if (fi == backupSettings.end())
        backupSettings.insert(make_pair(key, val));
    else
        fi->second = val;
}

BackupSettings::t_par BackupSettings::lineToPair(const std::string &line) {
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

std::string BackupSettings::pairToLine(const t_par &par) {
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

void BackupSettings::readSettings() {
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

void BackupSettings::setDefaultSettings() {
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
    v.push_back(make_pair(std::string("zip_cmd"),           std::string("\"%EXEC%\" a \"%BAK_DIR%\\%FILE_NAME%\" \"%SRC_DIR%\"")));
    v.push_back(make_pair(std::string("delete_old"),        std::string("false")));
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

void BackupSettings::writeSettings() {
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
