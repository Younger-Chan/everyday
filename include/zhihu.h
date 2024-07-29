#ifndef ZHIHU_H
#define ZHIHU_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>
#include <QDesktopServices>

namespace Ui {
class zhihu;
}

class zhihu : public QWidget
{
    Q_OBJECT

public:
    explicit zhihu(QWidget *parent = nullptr);
    ~zhihu();
    void initZhihu();

private:
    Ui::zhihu *ui;
    QNetworkAccessManager *networkZhihu;

private:

private slots:
    void onNetworkReplyZhihu(QNetworkReply *reply);

    void on_listWidget_itemClicked(QListWidgetItem *item);
};

#endif // ZHIHU_H
