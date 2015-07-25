#include "notifytray.h"

NotifyTray::NotifyTray(const QIcon &icon)
    : QSystemTrayIcon(icon), mClickToClose(false)
{
    QObject::connect(this, SIGNAL(messageClicked()),
                     this, SLOT(clickToClose()));
}

bool NotifyTray::getClickToClose() const {
    return mClickToClose;
}

void NotifyTray::setClickToClose(bool b) {
    mClickToClose = b;
}

void NotifyTray::clickToClose() {
    qDebug() << "Notify Tray Message Clicked";
    if (mClickToClose)
        emit onClickToClose();
}
