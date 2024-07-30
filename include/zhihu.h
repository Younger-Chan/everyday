#ifndef ZHIHU_H
#define ZHIHU_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QSvgRenderer>
#include <QPainter>

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
    QIcon loadSvgIcon(const QString &filePath, const QSize &size);

private:

private slots:
    void onNetworkReplyZhihu(QNetworkReply *reply);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
};

#endif // ZHIHU_H
