#include "weather.h"
#include "ui_weather.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

Weather::Weather(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Weather)
{
    ui->setupUi(this);
    // 创建一个网络访问管理器
    networkManager = new QNetworkAccessManager(this);

    // 当获取到响应时，调用onFinished处理
    connect(networkManager, &QNetworkAccessManager::finished, this, &Weather::onFinished);

    QString url_ip = "https://api.ipify.org?format=json";
    QNetworkRequest request_ip = QNetworkRequest(QUrl(url_ip));
    // 发起请求，获取公网IP
    networkManager->get(request_ip);

}

Weather::~Weather()
{
    delete ui;
}

void Weather::onFinished(QNetworkReply *reply)
{
    if (reply->error())
    {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

    // 读取响应数据
    QByteArray response_data = reply->readAll();

    // 将JSON字符串解析为QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data);

    // 提取IP地址
    QString ip = jsonDoc.object().value("ip").toString();

    networkLocation = new QNetworkAccessManager(this);
    QString host = "https://api.map.baidu.com";
    QString uri = "location/ip";
    QString coor = "bd09ll";
    QString ak = "MpRM3BBO3FZa8cSPnSmNGWDiICihCfuv";

    connect(networkLocation, &QNetworkAccessManager::finished, this, &Weather::onNetworkReplyLocation);

    QString url_loc = QString("%1/%2?ip=%3&coor=%4&ak=%5").arg(host, uri, ip, coor, ak);
    QNetworkRequest request_loc = QNetworkRequest(QUrl(url_loc));
    networkLocation->get(request_loc);
}

void Weather::onNetworkReplyLocation(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

        if(!jsonDoc.isNull())
        {
            QJsonObject jsonObj = jsonDoc.object();
            if(jsonObj["status"].toInt() == 0)
            {
                QJsonObject contentObj = jsonObj.value("content").toObject();
                QJsonObject addressDetailObj = contentObj.value("address_detail").toObject();
                QJsonObject pointObj = contentObj.value("point").toObject();

                QString displayText;
                // 提取city、province、x和y
                QString city = addressDetailObj.value("city").toString();
                QString province = addressDetailObj.value("province").toString();
                longitude = pointObj.value("x").toString();
                latitude = pointObj.value("y").toString();

                displayText += province + city;
                // qDebug() << longitude << "," << latitude;
                ui->l_loc->setText(displayText);
            }
            else
            {
                ui->l_loc->setText("Error: " + jsonObj["msg"].toString());
            }
        }
        else
        {
            ui->l_loc->setText("Error parsing JSON");
        }
    }
    else
    {
        ui->l_loc->setText("Network error: " + reply->errorString());
    }
    reply->deleteLater();

    networkWeather = new QNetworkAccessManager(this);

    connect(networkWeather, &QNetworkAccessManager::finished, this, &Weather::onNetworkReplyWeather);
    QString weatherApi = "https://api.caiyunapp.com";
    QString version = "v2.6";
    QString key = "3YEZULEHQHHXtUlI";

    QString url_weather = QString("%1/%2/%3/%4,%5/realtime").arg(weatherApi, version, key, longitude, latitude);
    // qDebug() << url_weather;
    QNetworkRequest request_weather = QNetworkRequest(QUrl(url_weather));
    networkWeather->get(request_weather);
}

void Weather::onNetworkReplyWeather(QNetworkReply *reply)
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

    // 获取根对象
    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject resultObj = jsonObj.value("result").toObject();
    QJsonObject realtimeObj = resultObj.value("realtime").toObject();

    // 提取所需数据
    double temperature = realtimeObj.value("temperature").toDouble();
    double feeling = realtimeObj.value("apparent_temperature").toDouble();
    double humidity = realtimeObj.value("humidity").toDouble();
    QString skycon = realtimeObj.value("skycon").toString();
    double windSpeed = realtimeObj.value("wind").toObject().value("speed").toDouble();
    double windDirection = realtimeObj.value("wind").toObject().value("direction").toDouble();
    QString chn = realtimeObj.value("air_quality").toObject().value("description").toObject().value("chn").toString();


    ui->l_temp->setText(QString::number(int(temperature)) + "℃");
    ui->l_sky->setText(skycon);
    ui->l_feelTempValue->setText(QString::number(int(feeling)) + "℃");
    ui->l_windValue->setText(windDirect(windDirection) + QString::number(windLevel(windSpeed)) + "级");
    ui->l_humidityValue->setText(QString::number(humidity * 100) + "%");
    ui->l_airValue->setText(chn);
}

int Weather::windLevel(const double windSpeed)
{
    if(windSpeed < 1.85)
        return 0;
    else if(windSpeed >= 1.85 && windSpeed < 5.55)
        return 1;
    else if(windSpeed >= 5.56 && windSpeed <= 11.1)
        return 2;
    else if(windSpeed >= 11.2 && windSpeed <= 19.4)
        return 3;
    else if(windSpeed >= 19.5 && windSpeed <= 28.4)
        return 4;
    else if(windSpeed >= 28.5 && windSpeed <= 40.7)
        return 5;
    else if(windSpeed >= 40.8 && windSpeed <= 49.9)
        return 6;
    else if(windSpeed >= 50.0 && windSpeed <= 61.1)
        return 7;
    else if(windSpeed >= 61.2 && windSpeed <= 74.4)
        return 8;
    else if(windSpeed >= 74.5 && windSpeed <= 87.6)
        return 9;
    else if(windSpeed >= 87.7 && windSpeed <= 101.9)
        return 10;
    else if(windSpeed >= 102.0 && windSpeed <= 117.9)
        return 11;
    else
        return 12;
}

QString Weather::windDirect(const double angle)
{
    if(angle >= 348.76 && angle <= 33.75)
        return "北风";
    else if(angle >= 33.76 && angle <= 78.75)
        return "东北风";
    else if(angle >= 78.76 && angle <= 123.75)
        return "东风";
    else if(angle >= 123.76 && angle <= 168.75)
        return "东南风";
    else if(angle >= 168.76 && angle <= 213.75)
        return "南风";
    else if(angle >= 213.76 && angle <= 258.75)
        return "西南风";
    else if(angle >= 258.76 && angle <= 303.75)
        return "西风";
    else
        return "西北风";
}
