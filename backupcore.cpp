#include "backupcore.h"

int BackupCore::currentVersion = 0;

int BackupCore::backup() {
    vec settings = readSettings();
    std::string zipCmd;//, moveCmd;
    resolveCmd(settings, zipCmd);
    QProcess process;
    process.start(zipCmd.c_str());
    if (!process.waitForStarted())
        return 1;
    if (!process.waitForFinished(-1))
        return 2;
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

BackupCore::vec BackupCore::readSettings() {
    vec settings;
    QFileInfo fileInfo("./config");
    if (fileInfo.exists() && fileInfo.isFile()) {
        std::ifstream fin;
        std::string str;
        fin.open("./config");
        // Get version first.
        int version = currentVersion;
        while (std::getline(fin, str))
            if (str.size() > 0 && str.at(0) != '#') {
                version = QString(str.c_str()).toInt();
                break;
            }
        if (version < currentVersion) {
            fin.close();
            updateSettings(version);
            fin.open("./config");
        }
        int seq = -1;
        while (std::getline(fin, str)) {
            if (str.size() > 0 && str.at(0) != '#') {
                settings.push_back(make_pair(getKey(++seq), str));
            }
        }
        fin.close();
    } else {
        setDefaultSettings(settings);
    }
    return settings;
}

void BackupCore::writeSettings(const vec &settings) {
    std::vector<std::string> outList;
    QFileInfo fileInfo("./config");
    if (fileInfo.exists() && fileInfo.isFile()) {
        std::ifstream fin;
        std::string str;
        fin.open("./config");
        int seq = -1;
        while (std::getline(fin, str)) {
            if (str.size() == 0 || str.at(0) == '#') {
                outList.push_back(str);
            } else {
                outList.push_back(settings.at(++seq).second);
            }
        }
        fin.close();
        std::ofstream fout;
        fout.open("./config");
        for (int i = 0; i < outList.size(); ++i) {
            fout << outList.at(i) << std::endl;
        }
        fout.close();
    }
}

void BackupCore::setDefaultSettings(vec &settings) {
    /*
     * 0 - version          - Settings file version
     * 1 - workspace_folder - Workspace folder
     * 2 - backup_folder    - Backup zip files folder
     * 3 - time_format      - Time format
     * 4 - zip_filename     - Zip file name format
     * 5 - zip_exe_path     - Zip command line software path
     * 6 - zip_cmd          - Zip exec cmd command
     * 7 - delete_old       - Whether delete out-of-date backup zip files
     */
    settings.clear();
    settings.push_back(make_pair(getKey(0), QString::number(currentVersion).toStdString()));
    settings.push_back(make_pair(getKey(1), "C:\\Workspaces"));
    settings.push_back(make_pair(getKey(2), "C:\\Workspaces_bak"));
    settings.push_back(make_pair(getKey(3), "%Y%m%d%H%M%S"));
    settings.push_back(make_pair(getKey(4), "Workspace_Bak_%TIME%.zip"));
    settings.push_back(make_pair(getKey(5), "C:\\Program Files\\WinRAR\\Rar.exe"));
    settings.push_back(make_pair(getKey(6), "%EXEC% a \"%BAK_DIR%\\%FILE_NAME%\" \"%SRC_DIR%\""));
    settings.push_back(make_pair(getKey(7), "true"));
}

void BackupCore::updateSettings(int oldVersion) {}

void BackupCore::resolveCmd(const vec &settings, std::string &cmd) {
    QString cm(settings.at(6).second.c_str()), cm_old;
    do {
        cm_old = cm;
        cm.replace("%EXEC%", QString(settings.at(5).second.c_str()));
        cm.replace("%BAK_DIR%", QString(settings.at(2).second.c_str()));
        cm.replace("%SRC_DIR%", QString(settings.at(1).second.c_str()));
        cm.replace("%FILE_NAME%", QString(settings.at(4).second.c_str()));
        cm.replace("%TIME%", QString(getCurrentTime(settings.at(3).second.c_str()).c_str()));
    } while (cm_old != cm);
    cmd = cm.toStdString();
}
