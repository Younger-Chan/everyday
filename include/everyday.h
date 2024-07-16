#ifndef EVERYDAY_H
#define EVERYDAY_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCloseEvent>

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
    QNetworkAccessManager *networkManager;
    void initSen();

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

private:
    Ui::EveryDay *ui;

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // EVERYDAY_H
