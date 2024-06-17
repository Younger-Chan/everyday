#include "everyday.h"
#include "ui_everyday.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

EveryDay::EveryDay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EveryDay)
{
    ui->setupUi(this);

    initSen();
}

EveryDay::~EveryDay()
{
    delete ui;
}

void EveryDay::initSen()
{
    ui->stackedWidget->setCurrentIndex(0);
    networkManager = new QNetworkAccessManager(this);

    QString url_sen = "https://v1.hitokoto.cn/?c=i&encode=json";// https://v1.hitokoto.cn/?c=f&encode=text   https://api.a20safe.com/api.php?api=6&key=6e64858a2dec587348d3ed9adaa0a66b&type=i
    QNetworkRequest request_sen = QNetworkRequest(QUrl(url_sen));
    connect(networkManager, &QNetworkAccessManager::finished, this, &EveryDay::onNetworkReply);

    networkManager->get(request_sen);
}

void EveryDay::on_pb_greet_clicked()
{
    initSen();
    ui->w_weather->initWeather();
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
            QString hitokoto = jsonObj["hitokoto"].toString();
            QString from = jsonObj["from"].toString();
            QString who = jsonObj["from_who"].toString();
            QString htmlText;
            htmlText += QString("<p>%1</p>").arg(hitokoto);
            htmlText += QString("<p>%1•《%2》</p>").arg(who, from);
            ui->l_greet->setText(htmlText);
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
    ui->stackedWidget_hot->setCurrentIndex(0);
    ui->page_weibo->initWeibo();
    ui->page_baidu->initBaidu();
    ui->page_douyin->initDouyin();
    ui->page_zhihu->initZhihu();
    ui->page_blbl->initBlbl();
    ui->page_wyy->initWyy();
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



