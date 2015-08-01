#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QString>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QObject>
#include <QTimer>
#include "notifytray.h"
#include "backupthread.h"

/*#ifdef Q_OS_WIN
#include "windows.h"

void pauseExec(int ms) {
    if (ms < 1)
        return;
    Sleep(uint(ms));
}
#endif*/

int main(int argc, char *argv[]) {
    if (argc > 1 && QString("--settings").compare(QString(argv[1])) == 0) {
        // Show setting windows GUI
        QApplication foregroundApp(argc, argv);
        MainWindow w;
        w.setWindowIcon(QIcon(":/icon.png"));
        w.setWindowTitle(QObject::tr("Code Backup Tool Settings"));
        w.show();
        return foregroundApp.exec();
    } else {
        // Just backup
        QApplication backgroundApp(argc, argv);
        backgroundApp.setQuitOnLastWindowClosed(false);
        NotifyTray notifyTrayIcon(QIcon(":/icon.png"));
        QObject::connect(&notifyTrayIcon, &NotifyTray::onClickToClose,
                         backgroundApp.quit);
        notifyTrayIcon.show();
        notifyTrayIcon.setClickToClose(false);
        notifyTrayIcon.showMessage(QObject::tr("Code Backup Tool"),
                                   QObject::tr("Backup starts..."));
        // Do backup...
        BackupThread * backupThread = new BackupThread();
        QTimer * quitTimer = new QTimer(&backgroundApp);
        quitTimer->setInterval(10000);
        QObject::connect(quitTimer, &QTimer::timeout, backgroundApp.quit);
        QObject::connect(backupThread, SIGNAL(resultReady(int)),
                         quitTimer, SLOT(start()));
        QObject::connect(backupThread, &BackupThread::resultReady,
                         [&notifyTrayIcon, &backgroundApp](int code) {
            notifyTrayIcon.setClickToClose(true);
            switch (code) {
            case 0:
                notifyTrayIcon.showMessage(QObject::tr("Code Backup Tool"),
                                           QObject::tr("Backup succeed!"));
                break;
            case 1:
                notifyTrayIcon.showMessage(QObject::tr("Code Backup Tool"),
                                           QObject::tr("Fail to start backup!"));
                break;
            case 2:
                notifyTrayIcon.showMessage(QObject::tr("Code Backup Tool"),
                                           QObject::tr("Fail to finish backup!"));
                break;
            default:
                break;
            }
        });
        QObject::connect(backupThread, SIGNAL(finished()),
                         backupThread, SLOT(deleteLater()));
        backupThread->start();
        return backgroundApp.exec();
    }
    // Impossible to reach. Just for dismissing the bothering warnings.
    return 0;
}
