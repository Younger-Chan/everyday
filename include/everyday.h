#ifndef EVERYDAY_H
#define EVERYDAY_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCloseEvent>
#include <QSvgRenderer>

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
    QNetworkAccessManager *networkManager;

private:
    void initSen();
    void initGui();
    QIcon loadSvgIcon(const QString &filePath, const QSize &size);

private slots:
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
