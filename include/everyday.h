#ifndef EVERYDAY_H
#define EVERYDAY_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCloseEvent>
#include <QSvgRenderer>
#include "customtitlebar.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class EveryDay;
}
QT_END_NAMESPACE

class EveryDay : public QMainWindow
{
    Q_OBJECT

public:
    EveryDay(QWidget *parent = nullptr);
    ~EveryDay();

private:
    Ui::EveryDay *ui;
    CustomTitleBar *bar;
    QNetworkAccessManager *networkManager;

    bool m_resizing = false;
    QPoint m_dragPosition;
    QRect m_startGeometry;

private:
    void initSen();
    void initGui();
    void resizeWindow(const QPoint &pos);
    void updateCursorShape(const QPoint &pos);
    QIcon loadSvgIcon(const QString &filePath, const QSize &size);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
    void toggleMaximizeRestore();

    void on_pb_greet_clicked();

    void on_pb_hot_clicked();

    void on_pb_todo_clicked();

    void on_pb_sub_clicked();

    void on_pb_me_clicked();

    void on_pb_down_clicked();

    void on_pb_up_clicked();

private slots:
    void onNetworkReply(QNetworkReply *reply);

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // EVERYDAY_H
