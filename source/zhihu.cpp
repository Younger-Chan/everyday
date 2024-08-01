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

QIcon zhihu::loadSvgIcon(const QString &filePath, const QSize &size)
{
    QSvgRenderer svgRenderer(filePath);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    svgRenderer.render(&painter);
    return QIcon(pixmap);
}

void zhihu::initZhihu()
{
    networkZhihu = new QNetworkAccessManager(this);
    QString url_hot = "https://api.suyanw.cn/api/zhihuresou.php"; // https://hot.cigh.cn/zhihu  https://api.suyanw.cn/api/zhihuresou.php
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

    // QJsonArray dataArray = jsonObj["data"].toArray();

    // for(const QJsonValue &value : dataArray)
    // {
    //     QJsonObject obj = value.toObject();
    //     QString title = obj["title"].toString();
    //     QString url = obj["url"].toString();

    //     int num = ui->listWidget->count() + 1;
    //     QString numStr = QString::number(num);
    //     QListWidgetItem *item = new QListWidgetItem(); // num + "." + title
    //     if(num < 4)
    //     {
    //         QString numFile = QString(":/number/number/number-%1.svg").arg(numStr);
    //         QIcon itemIcon = loadSvgIcon(numFile, QSize(40, 40));
    //         item->setIcon(itemIcon);
    //         item->setText(title);
    //     }
    //     else
    //     {
    //         item->setText(numStr + "." + title);
    //     }
    //     item->setData(Qt::UserRole, url);
    //     ui->listWidget->addItem(item);

    //     // vecHot << hotTitle;
    // }

    for(int i = 0; i < 30; i++)
    {
        QJsonObject topObj = jsonObj.value("Top_" + QString::number(i+1)).toObject();
        QString title = topObj["title"].toString();
        QString excerpt = topObj["excerpt"].toString();
        int num = i + 1;
        QString numStr = QString::number(num);
        QListWidgetItem *item = new QListWidgetItem();
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
        item->setToolTip(excerpt);
        ui->listWidget->addItem(item);
        //ui->listWidget->addItem(QString::number(i+1) + "." + title);
    }
    reply->deleteLater();
}

// void zhihu::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
// {
//     QString url = item->data(Qt::UserRole).toString();
//     if (!url.isEmpty()) {
//         QDesktopServices::openUrl(QUrl(url));
//     }
// }

