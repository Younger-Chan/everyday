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

    connect(networkWeibo, &QNetworkAccessManager::finished, this, &weibo::onNetworkReplyWeibo);
    QString url_hot = "https://api.a20safe.com/api.php?api=18&key=6e64858a2dec587348d3ed9adaa0a66b";
    QNetworkRequest request_hot = QNetworkRequest(QUrl(url_hot));
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

    QJsonObject dataObj = jsonObj.value("data").toObject();
    QJsonArray hotArray = dataObj.value("hot").toArray();
    for(const QJsonValue &value : hotArray)
    {
        QJsonObject hotObj = value.toObject();
        QString hotTitle = hotObj.value("title").toString();
        qDebug() << hotTitle;
        vecHot << hotTitle;
    }
    reply->deleteLater();
}
