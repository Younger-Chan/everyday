#ifndef BILIBILI_H
#define BILIBILI_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class bilibili;
}

class bilibili : public QWidget
{
    Q_OBJECT

public:
    explicit bilibili(QWidget *parent = nullptr);
    ~bilibili();
    void initBlbl();

private:
    Ui::bilibili *ui;
    QNetworkAccessManager *networkBlbl;

private:

private slots:
    void onNetworkReplyBlbl(QNetworkReply *reply);



};

#endif // BILIBILI_H
