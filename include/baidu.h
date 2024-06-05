#ifndef BAIDU_H
#define BAIDU_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class baidu;
}

class baidu : public QWidget
{
    Q_OBJECT

public:
    explicit baidu(QWidget *parent = nullptr);
    ~baidu();
    void initBaidu();

private:
    Ui::baidu *ui;
    QNetworkAccessManager *networkBaidu;

private:

private slots:
    void onNetworkReplyBaidu(QNetworkReply *reply);
};

#endif // BAIDU_H
