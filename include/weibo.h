#ifndef WEIBO_H
#define WEIBO_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QSvgRenderer>
#include <QPainter>

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
    QIcon loadSvgIcon(const QString &filePath, const QSize &size);

private:

private slots:
    void onNetworkReplyWeibo(QNetworkReply *reply);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
};

#endif // WEIBO_H
