#include "zhihu.h"
#include "ui_zhihu.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

zhihu::zhihu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::zhihu)
{
    ui->setupUi(this);
    // initZhihu();
}

zhihu::~zhihu()
{
    delete ui;
}

void zhihu::initZhihu()
{
    networkZhihu = new QNetworkAccessManager(this);
    QString url_hot = "https://api.suyanw.cn/api/zhihuresou.php";
    QNetworkRequest request_hot = QNetworkRequest(QUrl(url_hot));
    connect(networkZhihu, &QNetworkAccessManager::finished, this, &zhihu::onNetworkReplyZhihu);
    networkZhihu->get(request_hot);
}

void zhihu::onNetworkReplyZhihu(QNetworkReply *reply)
{
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

    for(int i = 0; i < 30; i++)
    {
        QJsonObject topObj = jsonObj.value("Top_" + QString::number(i+1)).toObject();
        QString hotTitle = topObj["title"].toString();
        ui->listWidget->addItem(QString::number(i+1) + "." + hotTitle);
    }
    reply->deleteLater();
}
