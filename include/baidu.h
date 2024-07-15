#ifndef BAIDU_H
#define BAIDU_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>
#include <QDesktopServices>

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
    void on_listWidget_itemClicked(QListWidgetItem *item);
};

#endif // BAIDU_H
