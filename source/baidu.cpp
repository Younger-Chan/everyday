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
    QString url_hot = "https://hot.cigh.cn/baidu";// https://api.suyanw.cn/api/bdrs.php?type=json  https://api.a20safe.com/api.php?api=17&key=6e64858a2dec587348d3ed9adaa0a66b
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
    // QJsonObject dataObj = dataArray[0].toObject();
    // QJsonArray hotArray = dataObj["content"].toArray();

    for(const QJsonValue &value : dataArray)
    {
        QJsonObject obj = value.toObject();
        QString title = obj["title"].toString();
        QString url = obj["url"].toString();

        QString num = QString::number(ui->listWidget->count() + 1);
        QListWidgetItem *item = new QListWidgetItem(num + "." + title);
        // QString numFile = QString(":/number/number/number-%1.svg").arg(num);
        // QIcon itemIcon = loadSvgIcon(numFile, QSize(40, 40));
        // item->setIcon(itemIcon);
        // item->setText(title);
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

void baidu::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString url = item->data(Qt::UserRole).toString();
    if (!url.isEmpty()) {
        QDesktopServices::openUrl(QUrl(url));
    }
}

