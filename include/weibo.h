#ifndef WEIBO_H
#define WEIBO_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class weibo;
}

class weibo : public QWidget
{
    Q_OBJECT

public:
    explicit weibo(QWidget *parent = nullptr);
    ~weibo();
    void initWeibo();

private:
    Ui::weibo *ui;
    QNetworkAccessManager *networkWeibo;

private:

private slots:
    void onNetworkReplyWeibo(QNetworkReply *reply);
};

#endif // WEIBO_H
