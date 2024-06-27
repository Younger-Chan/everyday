#include "douyin.h"
#include "ui_douyin.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

douyin::douyin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::douyin)
{
    ui->setupUi(this);
}

douyin::~douyin()
{
    delete ui;
}

void douyin::initDouyin()
{
    networkDouyin = new QNetworkAccessManager(this);
    QString url_hot = "https://api.vvhan.com/api/hotlist/douyinHot";
    QNetworkRequest request_hot = QNetworkRequest(QUrl(url_hot));
    connect(networkDouyin, &QNetworkAccessManager::finished, this, &douyin::onNetworkReplyDouyin);
    networkDouyin->get(request_hot);
}

void douyin::onNetworkReplyDouyin(QNetworkReply *reply)
{
    QVector<QString> vecHot;
    vecHot.clear();
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

    for(const QJsonValue &value : dataArray)
    {
        QJsonObject obj = value.toObject();
        QString hotTitle = obj["title"].toString();
        // QString url = obj["url"].toString();
        vecHot << hotTitle;
    }
    for(int i = 0; i < vecHot.size(); i++)
    {
        ui->listWidget->addItem(QString::number(i+1) + "." + vecHot[i]);
    }
    reply->deleteLater();
}
