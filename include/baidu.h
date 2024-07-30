#ifndef BAIDU_H
#define BAIDU_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QSvgRenderer>
#include <QPainter>

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
    QIcon loadSvgIcon(const QString &filePath, const QSize &size);

private:

private slots:
    void onNetworkReplyBaidu(QNetworkReply *reply);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
};

#endif // BAIDU_H
