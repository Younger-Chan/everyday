#ifndef EVERYDAY_H
#define EVERYDAY_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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
    // QNetworkAccessManager *networkManager_pic;

private slots:
    void on_pb_greet_clicked();

    void on_pb_hot_clicked();

    void on_pb_star_clicked();

    void on_pb_sub_clicked();

    void on_pb_me_clicked();

    void on_pb_down_clicked();

    void on_pb_up_clicked();

private slots:
    void onNetworkReply(QNetworkReply *reply);
    // void onNetworkReply_pic(QNetworkReply *reply);
    // QByteArray downloadImage(const QString &imageUrl);
    // void onImageReply();

private:
    Ui::EveryDay *ui;
};
#endif // EVERYDAY_H
