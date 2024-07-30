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

QIcon douyin::loadSvgIcon(const QString &filePath, const QSize &size)
{
    QSvgRenderer svgRenderer(filePath);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    svgRenderer.render(&painter);
    return QIcon(pixmap);
}

void douyin::initDouyin()
{
    networkDouyin = new QNetworkAccessManager(this);
    QString url_hot = "https://api.vvhan.com/api/hotlist/douyinHot";// https://hot.cigh.cn/douyin_new  https://api.vvhan.com/api/hotlist/douyinHot
    QNetworkRequest request_hot = QNetworkRequest(QUrl(url_hot));
    connect(networkDouyin, &QNetworkAccessManager::finished, this, &douyin::onNetworkReplyDouyin);
    networkDouyin->get(request_hot);
}

void douyin::onNetworkReplyDouyin(QNetworkReply *reply)
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
        QString url = obj["url"].toString();

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

void douyin::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString url = item->data(Qt::UserRole).toString();
    if (!url.isEmpty()) {
        QDesktopServices::openUrl(QUrl(url));
    }
}

