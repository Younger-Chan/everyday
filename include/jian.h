#ifndef JIAN_H
#define JIAN_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>

namespace Ui {
class jian;
}

class jian : public QWidget
{
    Q_OBJECT

public:
    explicit jian(QWidget *parent = nullptr);
    ~jian();
    void initJian();

private:
    Ui::jian *ui;
    QNetworkAccessManager *networkJian, *networkJianImg;

private:
    void loadJian(const QString);

private slots:
    void onNetworkReplyJian(QNetworkReply *reply);
    void onNetworkReplyJianImg(QNetworkReply *reply);
};

#endif // JIAN_H
