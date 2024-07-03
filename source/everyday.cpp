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
    // 获取应用程序的根目录
    QString rootDir = QCoreApplication::applicationDirPath();

    // 构建相对于根目录的sta.ini文件路径
    QString iniFilePath = QDir(rootDir).filePath("config/sentence/sentence.ini");
    QSettings settings(iniFilePath, QSettings::IniFormat);
    settings.beginGroup("dafult");
    QString name = settings.value("sentenceEn").toString();
    // QStringList namesList = names.split(",");
    // QString desc = settings.value("sentenceCh").toString();
    // QStringList descList = desc.split(",");

    settings.endGroup();


    ui->stackedWidget->setCurrentIndex(0);
    networkManager = new QNetworkAccessManager(this);

    QString url_base = "https://v1.hitokoto.cn/";
    QString url_type = QString("?c=%1").arg(name);
    QString url_encode = "&encode=json";
    QString url_sen = url_base + url_type + url_encode;
    // QString url_sen = "https://v1.hitokoto.cn/?c=i&encode=json";// https://v1.hitokoto.cn/?c=f&encode=text   https://api.a20safe.com/api.php?api=6&key=6e64858a2dec587348d3ed9adaa0a66b&type=i
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
            htmlText += QString("<p>%1•《%2》</p>").arg(who).arg(from);
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

void EveryDay::on_pb_todo_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void EveryDay::on_pb_sub_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->w_calendar->initCalendar();
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



