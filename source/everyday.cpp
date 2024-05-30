#include "everyday.h"
#include "./ui_everyday.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

EveryDay::EveryDay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EveryDay)
{
    ui->setupUi(this);
    // ui->stackedWidget->setCurrentIndex(0);

    networkManager = new QNetworkAccessManager(this);

    QString url_sen = "https://api.a20safe.com/api.php?api=6&key=6e64858a2dec587348d3ed9adaa0a66b&type=i";
    QNetworkRequest request_sen = QNetworkRequest(QUrl(url_sen));
    // QString url_pic = "https://api.a20safe.com/api.php?api=9&key=6e64858a2dec587348d3ed9adaa0a66b&lx=fengjing&hs=pc"; // https://api.suyanw.cn/api/scenery.php
    // QNetworkRequest request_pic = QNetworkRequest(QUrl(url_sen));

    connect(networkManager, &QNetworkAccessManager::finished, this, &EveryDay::onNetworkReply);

    networkManager->get(request_sen);
}

EveryDay::~EveryDay()
{
    delete ui;
}

void EveryDay::on_pb_greet_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    networkManager = new QNetworkAccessManager(this);

    QString url_sen = "https://api.a20safe.com/api.php?api=6&key=6e64858a2dec587348d3ed9adaa0a66b&type=i";
    QNetworkRequest request_sen = QNetworkRequest(QUrl(url_sen));
    // QString url_pic = "https://api.a20safe.com/api.php?api=9&key=6e64858a2dec587348d3ed9adaa0a66b&lx=fengjing&hs=pc"; // https://api.suyanw.cn/api/scenery.php
    // QNetworkRequest request_pic = QNetworkRequest(QUrl(url_sen));

    connect(networkManager, &QNetworkAccessManager::finished, this, &EveryDay::onNetworkReply);

    networkManager->get(request_sen);
    // networkManager->get(request_pic);
}

void EveryDay::onNetworkReply(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

        if(!jsonDoc.isNull())
        {
            QJsonObject jsonObj = jsonDoc.object();
            if(jsonObj["code"].toInt() == 0)
            {
                QJsonArray dataArray = jsonObj["data"].toArray();
                // QString displayText;
                QString htmlText;

                for(const QJsonValue &value : dataArray)
                {
                    QJsonObject obj = value.toObject();
                    // displayText += obj["motto"].toString() + "\n";
                    // displayText += "---《" + obj["from"].toString() + "》\n";
                    QString motto = obj["motto"].toString();
                    QString from = obj["from"].toString();

                    // 使用 HTML 标记语言来设置文本和图片
                    htmlText += QString("<p>%1</p>").arg(motto);
                    htmlText += QString("<p>---《%1》</p>").arg(from);
                }

                // QString imageUrl = "https://pic.a20safe.com/fengjing/pc/8429cee7aa0edd5139125aa313c59f69.jpg";
                // qDebug() << imageUrl;

                // QPixmap pixmap(imageUrl);
                // ui->l_greet->setPixmap(pixmap);
                ui->l_greet->setText(htmlText);
            }
            else
            {
                ui->l_greet->setText("Error: " + jsonObj["msg"].toString());
            }
        }
        else
        {
            ui->l_greet->setText("Error parsing JSON");
        }
    }
    else
    {
        ui->l_greet->setText("Network error: " + reply->errorString());
    }
    reply->deleteLater();
}

void EveryDay::on_pb_hot_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void EveryDay::on_pb_star_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void EveryDay::on_pb_sub_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void EveryDay::on_pb_me_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void EveryDay::on_pb_down_clicked()
{
    int currentIndex = ui->stackedWidget_hot->currentIndex();
    int nextIndex = (currentIndex + 1) % ui->stackedWidget_hot->count();
    ui->stackedWidget_hot->setCurrentIndex(nextIndex);
}

void EveryDay::on_pb_up_clicked()
{
    int currentIndex = ui->stackedWidget_hot->currentIndex();
    int prevIndex = (currentIndex - 1 + ui->stackedWidget_hot->count()) % ui->stackedWidget_hot->count();
    ui->stackedWidget_hot->setCurrentIndex(prevIndex);
}



