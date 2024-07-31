#include "wangyiy.h"
#include "ui_wangyiy.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

wangyiy::wangyiy(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wangyiy)
{
    ui->setupUi(this);
}

wangyiy::~wangyiy()
{
    delete ui;
}

void wangyiy::initWyy()
{
    networkWyy = new QNetworkAccessManager(this);
    QString url_hot = "https://api.suyanw.cn/api/wyrp_2.php";
    QNetworkRequest request_hot = QNetworkRequest(QUrl(url_hot));
    connect(networkWyy, &QNetworkAccessManager::finished, this, &wangyiy::onNetworkReplyWyy);
    networkWyy->get(request_hot);
}

void wangyiy::onNetworkReplyWyy(QNetworkReply *reply)
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

    QJsonObject obj = jsonObj.value("data").toObject();
    QString pic = obj.value("Picture").toString();
    QString music = obj.value("Music").toString();
    QString name = obj.value("name").toString();
    QString content = obj.value("Content").toString();
    QString nick = obj.value("Nick").toString();

    ui->l_music->setText(music + "-" + name);
    ui->l_content->setText(content + "-" + nick);

    reply->deleteLater();

    networkImg = new QNetworkAccessManager(this);
    connect(networkImg, &QNetworkAccessManager::finished, this, &wangyiy::onImageDownloaded);
    QUrl imageUrl(pic);
    QNetworkRequest request(imageUrl);
    networkImg->get(request);
}

void wangyiy::onImageDownloaded(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray imageData = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(imageData);

        // 获取QLabel的尺寸
        QSize labelSize = ui->l_pic->size();

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
        ui->l_pic->setScaledContents(true);
        ui->l_pic->setPixmap(scaledPixmap);
    }
    else
    {
        qWarning() << "Error downloading image:" << reply->errorString();
    }
    reply->deleteLater();
}
