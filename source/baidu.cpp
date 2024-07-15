#include "baidu.h"
#include "ui_baidu.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

baidu::baidu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::baidu)
{
    ui->setupUi(this);
    // initBaidu();
}

baidu::~baidu()
{
    delete ui;
}
void baidu::initBaidu()
{
    networkBaidu = new QNetworkAccessManager(this);
    QString url_hot = "https://api.suyanw.cn/api/bdrs.php?type=json";// https://api.suyanw.cn/api/bdrs.php?type=json  https://api.a20safe.com/api.php?api=17&key=6e64858a2dec587348d3ed9adaa0a66b
    QNetworkRequest request_hot = QNetworkRequest(QUrl(url_hot));
    connect(networkBaidu, &QNetworkAccessManager::finished, this, &baidu::onNetworkReplyBaidu);
    networkBaidu->get(request_hot);
}

void baidu::onNetworkReplyBaidu(QNetworkReply *reply)
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

    QJsonArray dataArray = jsonObj["cards"].toArray();
    QJsonObject dataObj = dataArray[0].toObject();
    QJsonArray hotArray = dataObj["content"].toArray();

    for(const QJsonValue &value : hotArray)
    {
        QJsonObject obj = value.toObject();
        QString hotTitle = obj["query"].toString();
        QString rawUrl = obj["rawUrl"].toString();

        QListWidgetItem *item = new QListWidgetItem(QString::number(ui->listWidget->count() + 1) + ". " + hotTitle);
        item->setData(Qt::UserRole, rawUrl);
        ui->listWidget->addItem(item);

        // vecHot << hotTitle;
    }
    // for(int i = 0; i < vecHot.size(); i++)
    // {
    //     ui->listWidget->addItem(QString::number(i+1) + "." + vecHot[i]);
    // }
    reply->deleteLater();
}

void baidu::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString url = item->data(Qt::UserRole).toString();
    if (!url.isEmpty()) {
        QDesktopServices::openUrl(QUrl(url));
    }
}

