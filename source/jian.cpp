#include "jian.h"
#include "ui_jian.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

jian::jian(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::jian)
{
    ui->setupUi(this);
}

jian::~jian()
{
    delete ui;
}

void jian::initJian()
{
    networkJian = new QNetworkAccessManager(this);
    QString url_hot = "https://dayu.qqsuu.cn/weiyujianbao/apis.php?type=json";
    QNetworkRequest request_hot = QNetworkRequest(QUrl(url_hot));
    connect(networkJian, &QNetworkAccessManager::finished, this, &jian::onNetworkReplyJian);
    networkJian->get(request_hot);
}

void jian::onNetworkReplyJian(QNetworkReply *reply)
{
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
    QString imageUrl = jsonObj["data"].toString(); // 获取第一个图片的 URL
    loadJian(imageUrl);
}

void jian::loadJian(const QString imgUrl)
{
    networkJianImg = new QNetworkAccessManager(this);
    QNetworkRequest request_my = QNetworkRequest(QUrl(imgUrl));
    connect(networkJianImg, &QNetworkAccessManager::finished, this, &jian::onNetworkReplyJianImg);
    networkJianImg->get(request_my);
}

void jian::onNetworkReplyJianImg(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());        // 获取QLabel的尺寸
        QSize labelSize = ui->l_60->size();

        // 计算控件的宽高比
        float labelAspectRatio = static_cast<float>(labelSize.width()) / labelSize.height();

        // 计算图像的宽高比
        float pixmapAspectRatio = static_cast<float>(pixmap.width()) / pixmap.height();

        // 计算最终图像尺寸，保持比例
        QSize finalSize;
        if (pixmapAspectRatio > labelAspectRatio) {
            // 如果图像的宽高比大于控件的宽高比，按控件宽度缩放
            finalSize.setWidth(labelSize.width());
            finalSize.setHeight(static_cast<int>(labelSize.width() / pixmapAspectRatio));
        } else {
            // 否则按控件高度缩放
            finalSize.setHeight(labelSize.height());
            finalSize.setWidth(static_cast<int>(labelSize.height() * pixmapAspectRatio));
        }
        QPixmap scaledPixmap = pixmap.scaled(finalSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->l_60->setScaledContents(true);
        ui->l_60->setPixmap(scaledPixmap);
    } else {
        ui->l_60->setText("Failed to load image.");
    }
    reply->deleteLater();
}
