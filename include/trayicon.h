#ifndef TRAYICON_H
#define TRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMainWindow>

class trayIcon : public QObject
{
    Q_OBJECT
public:
    trayIcon(QMainWindow *mainWindow);

private slots:
    void showMainWindow();
    void exitApplication();

private:
    QMainWindow *mainWindow;
    QSystemTrayIcon *tray;
    QMenu *trayMenu;
    QAction *showAction;
    QAction *exitAction;

};

#endif // TRAYICON_H
