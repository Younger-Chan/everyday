#ifndef DOUYIN_H
#define DOUYIN_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class douyin;
}

class douyin : public QWidget
{
    Q_OBJECT

public:
    explicit douyin(QWidget *parent = nullptr);
    ~douyin();
    void initDouyin();

private:
    Ui::douyin *ui;
    QNetworkAccessManager *networkDouyin;

private:

private slots:
    void onNetworkReplyDouyin(QNetworkReply *reply);
};

#endif // DOUYIN_H
