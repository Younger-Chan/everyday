#ifndef ZHIHU_H
#define ZHIHU_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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

};

#endif // ZHIHU_H
