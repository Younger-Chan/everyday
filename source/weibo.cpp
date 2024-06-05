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
    initWeibo();
}

weibo::~weibo()
{
    delete ui;
}

void weibo::initWeibo()
{
    networkWeibo = new QNetworkAccessManager(this);
    QString url_hot = "https://api.a20safe.com/api.php?api=18&key=6e64858a2dec587348d3ed9adaa0a66b";
    QNetworkRequest request_hot = QNetworkRequest(QUrl(url_hot));
    connect(networkWeibo, &QNetworkAccessManager::finished, this, &weibo::onNetworkReplyWeibo);
    networkWeibo->get(request_hot);
}

void weibo::onNetworkReplyWeibo(QNetworkReply *reply)
{
    QVector<QString> vecHot;

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
    QJsonObject dataObj = dataArray[0].toObject();
    QJsonArray hotArray = dataObj["hot"].toArray();

    for(const QJsonValue &value : hotArray)
    {
        QJsonObject obj = value.toObject();
        QString hotTitle = obj["title"].toString();
        vecHot << hotTitle;
    }
    for(int i = 0; i < vecHot.size(); i++)
    {
        ui->listWidget->addItem(QString::number(i+1) + "." + vecHot[i]);
    }
    reply->deleteLater();
}
