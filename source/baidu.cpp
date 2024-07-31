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

QIcon baidu::loadSvgIcon(const QString &filePath, const QSize &size)
{
    QSvgRenderer svgRenderer(filePath);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    svgRenderer.render(&painter);
    return QIcon(pixmap);
}

void baidu::initBaidu()
{
    networkBaidu = new QNetworkAccessManager(this);
    QString url_hot = "https://api.zhyunxi.com/api.php?api=17&key=6e64858a2dec587348d3ed9adaa0a66b";//  https://hot.cigh.cn/baidu https://api.suyanw.cn/api/bdrs.php?type=json  https://api.a20safe.com/api.php?api=17&key=6e64858a2dec587348d3ed9adaa0a66b
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

    QJsonArray dataArray = jsonObj["data"].toArray();
    QJsonObject dataObj = dataArray[0].toObject();
    QJsonArray topArray = dataObj["top"].toArray();
    QJsonArray hotArray = dataObj["hot"].toArray();

    QJsonObject topObj = topArray[0].toObject();
    QString title = topObj["query"].toString();
    QString url = topObj["rawUrl"].toString();
    QListWidgetItem *item = new QListWidgetItem();
    QString numFile = QString(":/number/number/number-top.svg");
    QIcon itemIcon = loadSvgIcon(numFile, QSize(40, 40));
    item->setIcon(itemIcon);
    item->setText(title);
    item->setData(Qt::UserRole, url);
    ui->listWidget->addItem(item);

    for(const QJsonValue &value : hotArray)
    {
        QJsonObject obj = value.toObject();
        QString title = obj["query"].toString();
        QString url = obj["rawUrl"].toString();

        int num = ui->listWidget->count();
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
    }
    reply->deleteLater();
}

void baidu::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString url = item->data(Qt::UserRole).toString();
    if (!url.isEmpty()) {
        QDesktopServices::openUrl(QUrl(url));
    }
}

