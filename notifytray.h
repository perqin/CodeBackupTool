#ifndef NOTIFYTRAY_H
#define NOTIFYTRAY_H

#include <QSystemTrayIcon>
#include <QObject>
#include <QIcon>
#include <QDebug>

class NotifyTray : public QSystemTrayIcon
{
    Q_OBJECT
signals:
    void onClickToClose();
private slots:
    void clickToClose();
public:
    NotifyTray(const QIcon & icon);
    bool getClickToClose() const;
    void setClickToClose(bool b);
private:
    bool mClickToClose;
};

#endif // NOTIFYTRAY_H
