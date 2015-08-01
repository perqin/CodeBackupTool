#include "backupcore.h"

int BackupCore::backup() {
    BackupSettings::readSettings();
    std::string zipCmd;
    resolveCmd(zipCmd);
    qDebug() << zipCmd.c_str();
//    QProcess process;
//    process.start(zipCmd.c_str());
//    if (!process.waitForStarted())
//        return 1;
//    if (!process.waitForFinished(-1))
//        return 2;
    Sleep(uint(8000));
    qDebug() << "finish";
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

std::string BackupCore::generateCmd(const std::string & cwv) {
    QString v_workspace_folder(BackupSettings::getValue("workspace_folder").c_str());
    QString v_backup_folder(BackupSettings::getValue("backup_folder").c_str());
    QString v_time_format(BackupSettings::getValue("time_format").c_str());
    QString v_zip_filename(BackupSettings::getValue("zip_filename").c_str());
    QString v_zip_exe_path(BackupSettings::getValue("zip_exe_path").c_str());
    QString cm(cwv.c_str());
    QString cm_old;
    do {
        cm_old = cm;
        cm.replace("%EXEC%", v_zip_exe_path);
        cm.replace("%BAK_DIR%", v_backup_folder);
        cm.replace("%SRC_DIR%", v_workspace_folder);
        cm.replace("%FILE_NAME%", v_zip_filename);
        cm.replace("%TIME%", QString(getCurrentTime(v_time_format.toStdString().c_str()).c_str()));
    } while (cm_old != cm);
    return cm.toStdString();
}

void BackupCore::resolveCmd(std::string &cmd) {
    cmd = generateCmd(BackupSettings::getValue("zip_cmd").c_str());
}
