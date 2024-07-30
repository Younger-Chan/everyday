#ifndef BILIBILI_H
#define BILIBILI_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>
#include <QDesktopServices>

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
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
};

#endif // BILIBILI_H
