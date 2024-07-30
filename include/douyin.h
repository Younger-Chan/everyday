#ifndef DOUYIN_H
#define DOUYIN_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>
#include <QDesktopServices>

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
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
};

#endif // DOUYIN_H
