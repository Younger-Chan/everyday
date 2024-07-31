#include "weibo.h"
#include "ui_weibo.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

weibo::weibo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::weibo)
{
    ui->setupUi(this);
    // initWeibo();
}

weibo::~weibo()
{
    delete ui;
}

QIcon weibo::loadSvgIcon(const QString &filePath, const QSize &size)
{
    QSvgRenderer svgRenderer(filePath);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    svgRenderer.render(&painter);
    return QIcon(pixmap);
}

void weibo::initWeibo()
{
    networkWeibo = new QNetworkAccessManager(this);
    // https://api.zhyunxi.com/api.php?api=18&key=6e64858a2dec587348d3ed9adaa0a66b
    // https://www.coderutil.com/api/resou/v1/weibo?size=50&&access-key=e20333a99e0d8414b87a9005d00dbfb8&secret-key=7549511dd170d796538c220b5366ac5f
    // QString accessKey = "e20333a99e0d8414b87a9005d00dbfb8";
    // QString secretKey = "7549511dd170d796538c220b5366ac5f";
    // QString url_base = "https://www.coderutil.com/api/resou/v1/weibo?size=50";
    // QString url_hot = QString("%1&&access-key=%2&secret-key=%3").arg(url_base, accessKey, secretKey);
    // https://hot.cigh.cn/weibo    // 不更新
    QString url_hot = "https://api.zhyunxi.com/api.php?api=18&key=6e64858a2dec587348d3ed9adaa0a66b";
    QNetworkRequest request_hot = QNetworkRequest(QUrl(url_hot));
    connect(networkWeibo, &QNetworkAccessManager::finished, this, &weibo::onNetworkReplyWeibo);
    networkWeibo->get(request_hot);
}

void weibo::onNetworkReplyWeibo(QNetworkReply *reply)
{
    // QVector<QString> vecHot;
    // vecHot.clear();
    ui->listWidget->clear();

    if (reply->error()) {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

    // 读取响应数据
    QByteArray response_data = reply->readAll();

    // 将JSON字符串解析为QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data);

    // 检查解析是否成功
    if (jsonDoc.isNull()) {
        qDebug() << "Failed to create JSON doc.";
        return;
    }
    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    QJsonArray dataArray = jsonObj["data"].toArray();
    QJsonObject jsonHot = dataArray[0].toObject();
    QJsonArray hotArray = jsonHot["hot"].toArray();

    for(const QJsonValue &value : hotArray)
    {
        QJsonObject obj = value.toObject();
        QString title = obj["title"].toString();
        QString url = obj["url"].toString();

        int num = ui->listWidget->count() + 1;
        QString numStr = QString::number(num);
        QListWidgetItem *item = new QListWidgetItem(); // num + "." + title
        if(num < 4)
        {
            QString numFile = QString(":/number/number/number-%1.svg").arg(numStr);
            QIcon itemIcon = loadSvgIcon(numFile, QSize(40, 40));
            item->setIcon(itemIcon);
            item->setText(title);
        }
        else
        {
            item->setText(numStr + "." + title);
        }
        item->setData(Qt::UserRole, url);
        ui->listWidget->addItem(item);

        // vecHot << hotTitle;
    }
    // for(int i = 0; i < vecHot.size(); i++)
    // {
    //     ui->listWidget->addItem(QString::number(i+1) + "." + vecHot[i]);
    // }
    reply->deleteLater();
}

void weibo::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString url = item->data(Qt::UserRole).toString();
    if (!url.isEmpty()) {
        QDesktopServices::openUrl(QUrl(url));
    }
}

