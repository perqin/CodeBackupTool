#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLayout>
#include <string>
#include "backupsettings.h"
#include "backupcore.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void saveSettings();
    void updatePreview(QString changedText);
private:
    QLabel * workspaceFolderLabel;
    QLineEdit * workspaceFolderEdit;
    QPushButton * workspaceFolderButton;
    QLabel * backupFolderLabel;
    QLineEdit * backupFolderEdit;
    QPushButton * backupFolderButton;
    QLabel * timeFormatLabel;
    QLineEdit * timeFormatEdit;
    QLabel * zipFilenameLabel;
    QLineEdit * zipFilenameEdit;
    QLabel * zipExePathLabel;
    QLineEdit * zipExePathEdit;
    QPushButton * zipExePathButton;
    QLabel * zipCmdLabel;
    QLineEdit * zipCmdEdit;
    QLabel * zipCmdHelp;
    QLabel * zipCmdPreview;
    QPushButton * saveButton;
    QPushButton * cancelButton;
};

#endif // MAINWINDOW_H
