#include "bilibili.h"
#include "ui_bilibili.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

bilibili::bilibili(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::bilibili)
{
    ui->setupUi(this);
    // initBlbl();
}

bilibili::~bilibili()
{
    delete ui;
}

QIcon bilibili::loadSvgIcon(const QString &filePath, const QSize &size)
{
    QSvgRenderer svgRenderer(filePath);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    svgRenderer.render(&painter);
    return QIcon(pixmap);
}

void bilibili::initBlbl()
{
    networkBlbl = new QNetworkAccessManager(this);
    QString url_hot = "https://api.suyanw.cn/api/bilibilirs.php";
    QNetworkRequest request_hot = QNetworkRequest(QUrl(url_hot));
    connect(networkBlbl, &QNetworkAccessManager::finished, this, &bilibili::onNetworkReplyBlbl);
    networkBlbl->get(request_hot);
}

void bilibili::onNetworkReplyBlbl(QNetworkReply *reply)
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

    for(const QJsonValue &value : dataArray)
    {
        QJsonObject obj = value.toObject();
        QString title = obj["title"].toString();
        QString link = obj["link"].toString();

        int num = ui->listWidget->count() + 1;
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
        item->setData(Qt::UserRole, link);
        ui->listWidget->addItem(item);

        // vecHot << hotTitle;
    }
    // for(int i = 0; i < vecHot.size(); i++)
    // {
    //     ui->listWidget->addItem(QString::number(i+1) + "." + vecHot[i]);
    // }
    reply->deleteLater();
}

void bilibili::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString url = item->data(Qt::UserRole).toString();
    if (!url.isEmpty()) {
        QDesktopServices::openUrl(QUrl(url));
    }
}

