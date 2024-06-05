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

    QString url_sen = "https://api.a20safe.com/api.php?api=6&key=6e64858a2dec587348d3ed9adaa0a66b&type=i";
    QNetworkRequest request_sen = QNetworkRequest(QUrl(url_sen));
    connect(networkManager, &QNetworkAccessManager::finished, this, &EveryDay::onNetworkReply);

    networkManager->get(request_sen);
}

void EveryDay::on_pb_greet_clicked()
{
    initSen();
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
    ui->stackedWidget_hot->setCurrentIndex(0);
    ui->page_weibo->initWeibo();
    ui->page_baidu->initBaidu();
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



