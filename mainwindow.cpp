#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      workspaceFolderLabel(new QLabel(tr("Workspace   "))),
      workspaceFolderEdit(new QLineEdit()),
      workspaceFolderButton(new QPushButton(tr("Browse..."))),
      backupFolderLabel(new QLabel(tr("Backup to   "))),
      backupFolderEdit(new QLineEdit()),
      backupFolderButton(new QPushButton(tr("Browse..."))),
      timeFormatLabel(new QLabel(tr("Time format "))),
      timeFormatEdit(new QLineEdit()),
      zipFilenameLabel(new QLabel(tr("Zip filename"))),
      zipFilenameEdit(new QLineEdit()),
      zipExePathLabel(new QLabel(tr("Zip exe path"))),
      zipExePathEdit(new QLineEdit()),
      zipExePathButton(new QPushButton(tr("Browse..."))),
      zipCmdLabel(new QLabel(tr("Zip command "))),
      zipCmdEdit(new QLineEdit()),
      zipCmdHelp(new QLabel()),
      zipCmdPreview(new QLabel()),
      saveButton(new QPushButton(tr("Save changes"))),
      cancelButton(new QPushButton(tr("Discard changes")))
{
    // Layouts
    QHBoxLayout * workspaceFolderLayout = new QHBoxLayout();
    workspaceFolderLayout->addWidget(workspaceFolderLabel);
    workspaceFolderLayout->addWidget(workspaceFolderEdit);
    workspaceFolderLayout->addWidget(workspaceFolderButton);
    QHBoxLayout * backupFolderLayout = new QHBoxLayout();
    backupFolderLayout->addWidget(backupFolderLabel);
    backupFolderLayout->addWidget(backupFolderEdit);
    backupFolderLayout->addWidget(backupFolderButton);
    QHBoxLayout * timeFormatLayout = new QHBoxLayout();
    timeFormatLayout->addWidget(timeFormatLabel);
    timeFormatLayout->addWidget(timeFormatEdit);
    timeFormatLayout->addWidget(zipFilenameLabel);
    timeFormatLayout->addWidget(zipFilenameEdit);
    QHBoxLayout * zipExePathLayout = new QHBoxLayout();
    zipExePathLayout->addWidget(zipExePathLabel);
    zipExePathLayout->addWidget(zipExePathEdit);
    zipExePathLayout->addWidget(zipExePathButton);
    QHBoxLayout * zipCmdLayout = new QHBoxLayout();
    zipCmdLayout->addWidget(zipCmdLabel);
    zipCmdLayout->addWidget(zipCmdEdit);
    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(cancelButton);
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->addLayout(workspaceFolderLayout);
    mainLayout->addLayout(backupFolderLayout);
    mainLayout->addLayout(timeFormatLayout);
    mainLayout->addLayout(zipExePathLayout);
    mainLayout->addLayout(zipCmdLayout);
    mainLayout->addWidget(zipCmdHelp);
    mainLayout->addWidget(zipCmdPreview);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    QWidget * cw = new QWidget();
    cw->setLayout(mainLayout);
    setCentralWidget(cw);

    // UI
    zipCmdHelp->setWordWrap(true);
    zipCmdHelp->setText(tr("Variables:\n%EXEC%      - Zip exe path\n%BAK_DIR%   - Backup folder\n%SRC_DIR%   - Workspace folder\n%FILE_NAME% - Zip filename\n%TIME%      - Time string\nCommand preview:"));
    zipCmdPreview->setWordWrap(true);

    // Read settings
    BackupSettings::readSettings();
    workspaceFolderEdit->setText(BackupSettings::getValue("workspace_folder").c_str());
    backupFolderEdit->setText(BackupSettings::getValue("backup_folder").c_str());
    timeFormatEdit->setText(BackupSettings::getValue("time_format").c_str());
    zipFilenameEdit->setText(BackupSettings::getValue("zip_filename").c_str());
    zipExePathEdit->setText(BackupSettings::getValue("zip_exe_path").c_str());
    zipCmdEdit->setText(BackupSettings::getValue("zip_cmd").c_str());

    // Connections
    QObject::connect(zipCmdEdit, SIGNAL(textChanged(QString)),
                     this, SLOT(updatePreview(QString)));
    QObject::connect(saveButton, SIGNAL(clicked(bool)),
                     this, SLOT(saveSettings()));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)),
                     this, SLOT(close()));
    emit zipCmdEdit->textChanged(zipCmdEdit->text());
}

MainWindow::~MainWindow() {}

void MainWindow::saveSettings() {
    BackupSettings::setValue("workspace_folder", workspaceFolderEdit->text().toStdString());
    BackupSettings::setValue("backup_folder", backupFolderEdit->text().toStdString());
    BackupSettings::setValue("time_format", timeFormatEdit->text().toStdString());
    BackupSettings::setValue("zip_filename", zipFilenameEdit->text().toStdString());
    BackupSettings::setValue("zip_exe_path", zipExePathEdit->text().toStdString());
    BackupSettings::setValue("zip_cmd", zipCmdEdit->text().toStdString());
    BackupSettings::writeSettings();
    hide();
}

void MainWindow::updatePreview(QString changedText) {
    std::string cmd = BackupCore::generateCmd(changedText.toStdString());
    zipCmdPreview->setText(cmd.c_str());
}
