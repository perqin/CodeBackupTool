#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QString>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QObject>
#include "notifytray.h"

#ifdef Q_OS_WIN
#include "windows.h"

void pauseExec(int ms) {
    if (ms < 1)
        return;
    Sleep(uint(ms));
}
#endif

int main(int argc, char *argv[]) {
    if (argc > 1 && QString("--setting").compare(QString(argv[1])) == 0) {
        // Show setting windows GUI
        QApplication foregroundApp(argc, argv);
        MainWindow w;
        w.show();
        return foregroundApp.exec();
    } else {
        // Just backup
        QApplication backgroundApp(argc, argv);
        NotifyTray notifyTrayIcon(QIcon(":/icon.png"));
        QObject::connect(&notifyTrayIcon, &NotifyTray::onClickToClose,
                         backgroundApp.quit);
        //QObject::connect(&notifyTrayIcon, &NotifyTray::messageClicked,
        //                 backgroundApp.quit);
        notifyTrayIcon.show();
        notifyTrayIcon.setClickToClose(false);
        notifyTrayIcon.showMessage(QObject::tr("Code Backup Tool"),
                                   QObject::tr("Backup starts..."));
        // Do backup...
        //
        pauseExec(3000);
        notifyTrayIcon.setClickToClose(true);
        notifyTrayIcon.showMessage(QObject::tr("Code Backup Tool"), QObject::tr("Backup finish. Click here to close."));
        pauseExec(30000);
        backgroundApp.quit();
    }
    // Impossible to reach. Just for dismissing the bothering warnings.
    return 0;
}
