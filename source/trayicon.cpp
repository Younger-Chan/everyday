#include "trayicon.h"
#include <QApplication>

trayIcon::trayIcon(QMainWindow *mainWindow)
    : mainWindow(mainWindow)
{
    tray = new QSystemTrayIcon(mainWindow);
    tray->setIcon(QIcon(":/other/other/evy.png"));
    tray->setToolTip("每天");

    trayMenu = new QMenu(mainWindow);
    showAction = new QAction("打开", trayMenu);
    exitAction = new QAction("退出", trayMenu);

    trayMenu->addAction(showAction);
    trayMenu->addAction(exitAction);

    tray->setContextMenu(trayMenu);
    tray->show();

    connect(tray, &QSystemTrayIcon::activated, this, [&](QSystemTrayIcon::ActivationReason reason)
    {
        if(reason == QSystemTrayIcon::Trigger)
        {
            mainWindow->show();
        }
        else if(reason == QSystemTrayIcon::Context)
        {
            // 右键单击弹出菜单
            trayMenu->popup(QCursor::pos());
        }
    });

    connect(showAction, &QAction::triggered, this, &trayIcon::showMainWindow);
    connect(exitAction, &QAction::triggered, this, &trayIcon::exitApplication);
    // connect(tray, &QSystemTrayIcon::activated, this, &trayIcon::showMainWindow);
}

void trayIcon::showMainWindow()
{
    mainWindow->show();
    mainWindow->raise();
    mainWindow->activateWindow();
}

void trayIcon::exitApplication()
{
    QApplication::quit();
}

