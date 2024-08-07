#include "sentence.h"
#include "ui_sentence.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

sentence::sentence(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::sentence)
{
    ui->setupUi(this);
}

sentence::~sentence()
{
    delete ui;
}

void sentence::initSen()
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


    // ui->stackedWidget->setCurrentIndex(0);
    networkManager = new QNetworkAccessManager(this);

    QString url_base = "https://v1.hitokoto.cn/";
    QString url_type = QString("?c=%1").arg(name);
    QString url_encode = "&encode=json";
    QString url_sen = url_base + url_type + url_encode;
    // QString url_sen = "https://v1.hitokoto.cn/?c=i&encode=json";// https://v1.hitokoto.cn/?c=f&encode=text   https://api.a20safe.com/api.php?api=6&key=6e64858a2dec587348d3ed9adaa0a66b&type=i
    QNetworkRequest request_sen = QNetworkRequest(QUrl(url_sen));
    connect(networkManager, &QNetworkAccessManager::finished, this, &sentence::onNetworkReply);

    networkManager->get(request_sen);
}

void sentence::onNetworkReply(QNetworkReply *reply)
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
            ui->label->setText(htmlText);
        }
        else
        {
            ui->label->setText("Error parsing JSON");
        }
    }
    else
    {
        ui->label->setText("Network error: " + reply->errorString());
    }
    reply->deleteLater();
}
