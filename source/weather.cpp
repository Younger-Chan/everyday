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

    mapSkyconIcon =
    {
        {"CLEAR_DAY", ":weather/weather/qing.png"},
        {"CLEAR_NIGHT", ":weather/weather/qing.png"},
        {"PARTLY_CLOUDY_DAY", ":weather/weather/duoyun.png"},
        {"PARTLY_CLOUDY_NIGHT", ":weather/weather/duoyun.png"},
        {"CLOUDY",":weather/weather/yin.png"},
        {"LIGHT_HAZE", ":weather/weather/wumai.png"},
        {"MODERATE_HAZE", ":weather/weather/wumai.png"},
        {"HEAVY_HAZE", ":weather/weather/wumai.png"},
        {"LIGHT_RAIN", ":weather/weather/xiaoyu.png"},
        {"MODERATE_RAIN", ":weather/weather/zhongyu.png"},
        {"HEAVY_RAIN", ":weather/weather/dayu.png"},
        {"STORM_RAIN", ":weather/weather/baoyu.png"},
        {"FOG", ":weather/weather/wu.png"},
        {"LIGHT_SNOW", ":weather/weather/xiaoxue.png"},
        {"MODERATE_SNOW", ":weather/weather/zhongxue.png"},
        {"HEAVY_SNOW", ":weather/weather/daxue.png"},
        {"STORM_SNOW", ":weather/weather/dabaoxue.png"},
        {"DUST", ":weather/weather/fuchen.png"},
        {"SAND", ":weather/weather/shachen.png"},
        {"WIND", ":weather/weather/feng.png"}
    };
    initWeather();
}

Weather::~Weather()
{
    delete ui;
}

void Weather::initWeather()
{

    // 创建一个网络访问管理器
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &Weather::onNetworkReplyIp);

    QString url_ip = "https://api.ipify.org?format=json";
    QNetworkRequest request_ip = QNetworkRequest(QUrl(url_ip));
    networkManager->get(request_ip);

}

void Weather::onNetworkReplyIp(QNetworkReply *reply)
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
    QString daily = "daily?dailysteps";
    QString dailysteps = "3";

    QString url_weather = QString("%1/%2/%3/%4,%5/realtime").arg(weatherApi, version, key, longitude, latitude);
    // qDebug() << url_weather;
    QNetworkRequest request_weather = QNetworkRequest(QUrl(url_weather));
    networkWeather->get(request_weather);

    networkMutiWeather = new QNetworkAccessManager(this);
    connect(networkMutiWeather, &QNetworkAccessManager::finished, this, &Weather::onNetworkReplyMutiWeather);
    QString url_mutiWeather = QString("%1/%2/%3/%4,%5/%6=%7").arg(weatherApi, version, key, longitude, latitude, daily, dailysteps);
    QNetworkRequest request_mutiWeather = QNetworkRequest(QUrl(url_mutiWeather));
    networkMutiWeather->get(request_mutiWeather);
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
    ui->l_sky->setText(skyCondition(skycon));
    ui->l_feelTempValue->setText(QString::number(int(feeling)) + "℃");
    ui->l_windValue->setText(windDirect(windDirection) + QString::number(windLevel(windSpeed)) + "级");
    ui->l_humidityValue->setText(QString::number(humidity * 100) + "%");
    ui->l_airValue->setText(chn);
}

void Weather::onNetworkReplyMutiWeather(QNetworkReply *reply)
{
    QVector<QString> vecDate;
    QVector<double> vecMaxTemp;
    QVector<double> vecMinTemp;
    QVector<QString> vecSkycon;

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
    QJsonObject dailyObj = resultObj.value("daily").toObject();
    QJsonArray tempArray = dailyObj.value("temperature").toArray();
    QJsonArray skyArray = dailyObj.value("skycon").toArray();

    // 遍历数组中的每个对象并提取内容
    for(const QJsonValue &value : tempArray)
    {
        QJsonObject tempObj = value.toObject();
        QString date = tempObj.value("date").toString();
        double max = tempObj.value("max").toDouble();
        double min = tempObj.value("min").toDouble();

        vecDate << date;
        vecMaxTemp << max;
        vecMinTemp << min;
    }

    for(const QJsonValue &value : skyArray)
    {
        QJsonObject skyObj = value.toObject();
        QString sky = skyObj.value("value").toString();

        vecSkycon << sky;
    }

    ui->l_tod_date->setText("今天"); // vecDate[0].left(10)
    ui->l_tom_date->setText("明天"); // vecDate[1].left(10)
    ui->l_aft_date->setText("后天"); // vecDate[2].left(10)

    ui->l_tod_temp->setText(QString::number(round(vecMinTemp[0])) + "℃" + "~" + QString::number(round(vecMaxTemp[0])) + "℃");
    ui->l_tom_temp->setText(QString::number(round(vecMinTemp[1])) + "℃" + "~" + QString::number(round(vecMaxTemp[1])) + "℃");
    ui->l_aft_temp->setText(QString::number(round(vecMinTemp[2])) + "℃" + "~" + QString::number(round(vecMaxTemp[2])) + "℃");

    QPixmap pixmap_tod(mapSkyconIcon[vecSkycon[0]]);
    ui->l_tod_icon->setPixmap(pixmap_tod.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->l_tod_icon->setScaledContents(true); // 确保内容被缩放

    QPixmap pixmap_tom(mapSkyconIcon[vecSkycon[1]]);
    ui->l_tom_icon->setPixmap(pixmap_tom.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->l_tom_icon->setScaledContents(true); // 确保内容被缩放

    QPixmap pixmap_aft(mapSkyconIcon[vecSkycon[2]]);
    ui->l_aft_icon->setPixmap(pixmap_aft.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->l_aft_icon->setScaledContents(true); // 确保内容被缩放

    ui->l_tod_condition->setText(skyCondition(vecSkycon[0]));
    ui->l_tom_condition->setText(skyCondition(vecSkycon[1]));
    ui->l_aft_condition->setText(skyCondition(vecSkycon[2]));

}

QString Weather::skyCondition(const QString sky)
{
    if(sky == "CLEAR_DAY")
        return "晴";
    else if(sky == "CLEAR_NIGHT")
        return "晴";
    else if(sky == "PARTLY_CLOUDY_DAY")
        return "多云";
    else if(sky == "PARTLY_CLOUDY_NIGHT")
        return "多云";
    else if(sky == "CLOUDY")
        return "阴";
    else if(sky == "LIGHT_HAZE")
        return "轻度雾霾";
    else if(sky == "MODERATE_HAZE")
        return "中度雾霾";
    else if(sky == "HEAVY_HAZE")
        return "重度雾霾";
    else if(sky == "LIGHT_RAIN")
        return "小雨";
    else if(sky == "MODERATE_RAIN")
        return "中雨";
    else if(sky == "HEAVY_RAIN")
        return "大雨";
    else if(sky == "STORM_RAIN")
        return "暴雨";
    else if(sky == "FOG")
        return "雾";
    else if(sky == "LIGHT_SNOW")
        return "小雪";
    else if(sky == "MODERATE_SNOW")
        return "中雪";
    else if(sky == "HEAVY_SNOW")
        return "大雪";
    else if(sky == "STORM_SNOW")
        return "暴雪";
    else if(sky == "DUST")
        return "浮尘";
    else if(sky == "SAND")
        return "沙尘";
    else
        return "大风";
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
