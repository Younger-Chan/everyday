#ifndef WANGYIY_H
#define WANGYIY_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class wangyiy;
}

class wangyiy : public QWidget
{
    Q_OBJECT

public:
    explicit wangyiy(QWidget *parent = nullptr);
    ~wangyiy();
    void initWyy();

private:
    Ui::wangyiy *ui;
    QNetworkAccessManager *networkWyy, *networkImg;

private:

private slots:
    void onNetworkReplyWyy(QNetworkReply *reply);
    void onImageDownloaded(QNetworkReply *reply);


};

#endif // WANGYIY_H
