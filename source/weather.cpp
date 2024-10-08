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
        // {"CLEAR_DAY", ":weather/weather/qing.png"},
        // {"CLEAR_NIGHT", ":weather/weather/qing.png"},
        // {"PARTLY_CLOUDY_DAY", ":weather/weather/duoyun.png"},
        // {"PARTLY_CLOUDY_NIGHT", ":weather/weather/duoyun.png"},
        // {"CLOUDY",":weather/weather/yin.png"},
        // {"LIGHT_HAZE", ":weather/weather/wumai.png"},
        // {"MODERATE_HAZE", ":weather/weather/wumai.png"},
        // {"HEAVY_HAZE", ":weather/weather/wumai.png"},
        // {"LIGHT_RAIN", ":weather/weather/xiaoyu.png"},
        // {"MODERATE_RAIN", ":weather/weather/zhongyu.png"},
        // {"HEAVY_RAIN", ":weather/weather/dayu.png"},
        // {"STORM_RAIN", ":weather/weather/baoyu.png"},
        // {"FOG", ":weather/weather/wu.png"},
        // {"LIGHT_SNOW", ":weather/weather/xiaoxue.png"},
        // {"MODERATE_SNOW", ":weather/weather/zhongxue.png"},
        // {"HEAVY_SNOW", ":weather/weather/daxue.png"},
        // {"STORM_SNOW", ":weather/weather/dabaoxue.png"},
        // {"DUST", ":weather/weather/fuchen.png"},
        // {"SAND", ":weather/weather/shachen.png"},
        // {"WIND", ":weather/weather/feng.png"}

        {"CLEAR_DAY", ":weather_svg/weather_svg/qing.svg"},
        {"CLEAR_NIGHT", ":weather_svg/weather_svg/qing_yue.svg"},
        {"PARTLY_CLOUDY_DAY", ":weather_svg/weather_svg/duoyun.svg"},
        {"PARTLY_CLOUDY_NIGHT", ":weather_svg/weather_svg/duoyun_yue.svg"},
        {"CLOUDY",":weather_svg/weather_svg/yin.svg"},
        {"LIGHT_HAZE", ":weather_svg/weather_svg/wumai.svg"},
        {"MODERATE_HAZE", ":weather_svg/weather_svg/wumai.svg"},
        {"HEAVY_HAZE", ":weather_svg/weather_svg/wumai.svg"},
        {"LIGHT_RAIN", ":weather_svg/weather_svg/xiaoyu.svg"},
        {"MODERATE_RAIN", ":weather_svg/weather_svg/zhongyu.svg"},
        {"HEAVY_RAIN", ":weather_svg/weather_svg/dayu.svg"},
        {"STORM_RAIN", ":weather_svg/weather_svg/baoyu.svg"},
        {"FOG", ":weather_svg/weather_svg/wu.svg"},
        {"LIGHT_SNOW", ":weather_svg/weather_svg/xiaoxue.svg"},
        {"MODERATE_SNOW", ":weather_svg/weather_svg/zhongxue.svg"},
        {"HEAVY_SNOW", ":weather_svg/weather_svg/daxue.svg"},
        {"STORM_SNOW", ":weather_svg/weather_svg/baoxue.svg"},
        {"DUST", ":weather_svg/weather_svg/fuchen.svg"},
        {"SAND", ":weather_svg/weather_svg/shachen.svg"},
        {"WIND", ":weather_svg/weather_svg/feng.svg"}
    };
    initWeather();
}

Weather::~Weather()
{
    delete ui;
}


QPixmap Weather::loadSvgIcon(const QString &filePath, const QSize &size)
{
    QSvgRenderer svgRenderer(filePath);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    svgRenderer.render(&painter);
    return pixmap;
}


void Weather::initWeather()
{
    QPixmap svgLocIcon = loadSvgIcon(":/tool/tool/map_pin_human_fill.svg",  QSize(40, 40));
    ui->l_locIcon->setPixmap(svgLocIcon);

    // 创建一个网络访问管理器
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &Weather::onNetworkReplyIp);

    QString url_ip = "https://ip.useragentinfo.com/json?ip";// http://api.ipify.org?format=json   http://httpbin.org/ip  https://ip.useragentinfo.com/json?ip=
    QNetworkRequest request_ip = QNetworkRequest(QUrl(url_ip));
    networkManager->get(request_ip);

}

void Weather::onNetworkReplyIp(QNetworkReply *reply)
{
    if (reply->error())
    {
        // qDebug() << "Error:" << reply->errorString();
        return;
    }

    // 读取响应数据
    QByteArray response_data = reply->readAll();

    // 将JSON字符串解析为QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data);
    QJsonObject jsonObj = jsonDoc.object();
    // 提取IP地址
    // QString ip = jsonDoc.object().value("origin").toString();
    QString ip = jsonObj.value("ip").toString();
    // QString country = jsonObj.value("country").toString();
    // QString province = jsonObj.value("province").toString();
    // QString city = jsonObj.value("city").toString();
    // QString area = jsonObj.value("area").toString();

    reply->deleteLater();

    networkLocation = new QNetworkAccessManager(this);
    QString host = "https://api.suyanw.cn/api/ipxx.php";

    connect(networkLocation, &QNetworkAccessManager::finished, this, &Weather::onNetworkReplyLocation);
    QString url_loc = QString("%1?ip=%2").arg(host, ip);
    QNetworkRequest request_loc = QNetworkRequest(QUrl(url_loc));
    networkLocation->get(request_loc);
}

void Weather::onNetworkReplyLocation(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject dataObj = jsonObj.value("data").toObject();
    QString province = dataObj["province"].toString();
    QString city = dataObj["city"].toString();
    QString district = dataObj["district"].toString();
    longitude = dataObj["lng"].toString();
    latitude = dataObj["lat"].toString();
    QString displayText = province + city + district;
    ui->l_loc->setText(displayText);
    reply->deleteLater();

    networkWeather = new QNetworkAccessManager(this);
    connect(networkWeather, &QNetworkAccessManager::finished, this, &Weather::onNetworkReplyWeather);
    QString weatherApi = "https://api.caiyunapp.com";
    QString version = "v2.6";
    QString key = "3YEZULEHQHHXtUlI";
    QString daily = "daily?dailysteps";
    QString hourly = "hourly?hourlysteps";
    QString weatherAlert = "alert";
    QString dailysteps = "3";
    QString hourlysteps = "1";

    QString url_weather = QString("%1/%2/%3/%4,%5/realtime?%6=true").arg(weatherApi, version, key, longitude, latitude, weatherAlert);
    QNetworkRequest request_weather = QNetworkRequest(QUrl(url_weather));
    networkWeather->get(request_weather);

    networkMutiWeather = new QNetworkAccessManager(this);
    connect(networkMutiWeather, &QNetworkAccessManager::finished, this, &Weather::onNetworkReplyMutiWeather);
    QString url_mutiWeather = QString("%1/%2/%3/%4,%5/%6=%7").arg(weatherApi, version, key, longitude, latitude, daily, dailysteps);
    QNetworkRequest request_mutiWeather = QNetworkRequest(QUrl(url_mutiWeather));
    networkMutiWeather->get(request_mutiWeather);

    networkHourlyWeather = new QNetworkAccessManager(this);
    connect(networkHourlyWeather, &QNetworkAccessManager::finished, this, &Weather::onNetworkReplyHourlyWeather);
    QString url_hourlyWeather = QString("%1/%2/%3/%4,%5/%6=%7").arg(weatherApi, version, key, longitude, latitude, hourly, hourlysteps);
    QNetworkRequest request_hourlyWeather = QNetworkRequest(QUrl(url_hourlyWeather));
    networkHourlyWeather->get(request_hourlyWeather);
}

void Weather::onNetworkReplyWeather(QNetworkReply *reply)
{
    if (reply->error()) {
        // qDebug() << "Error:" << reply->errorString();
        return;
    }

    // 读取响应数据
    QByteArray response_data = reply->readAll();

    // 将JSON字符串解析为QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data);

    // 检查解析是否成功
    if (jsonDoc.isNull()) {
        // qDebug() << "Failed to create JSON doc.";
        return;
    }
    if (!jsonDoc.isObject()) {
        // qDebug() << "JSON is not an object.";
        return;
    }

    // 获取根对象
    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject resultObj = jsonObj.value("result").toObject();
    QJsonObject realtimeObj = resultObj.value("realtime").toObject();
    QJsonObject airObj = realtimeObj.value("air_quality").toObject();
    QJsonObject descObj = airObj.value("description").toObject();

    if(!jsonObj.contains("alert"))
    {
        ui->l_alert->setText("暂无预警信息");
    }
    else
    {
        QJsonObject alertObj = realtimeObj.value("alert").toObject();
        QJsonArray alertArray = alertObj.value("content").toArray();
        QJsonObject firstAlert = alertArray.at(0).toObject();
        QString title = firstAlert.value("title").toString();
        ui->l_alert->setText(title);
    }

    // 提取所需数据
    double temperature = realtimeObj.value("temperature").toDouble();
    double feeling = realtimeObj.value("apparent_temperature").toDouble();
    double humidity = realtimeObj.value("humidity").toDouble();
    QString skycon = realtimeObj.value("skycon").toString();
    double windSpeed = realtimeObj.value("wind").toObject().value("speed").toDouble();
    double windDirection = realtimeObj.value("wind").toObject().value("direction").toDouble();
    QString chn = descObj.value("chn").toString();

    ui->l_temp->setText( QString::number(int(temperature)) + "℃");

    QPixmap svgIcon = loadSvgIcon(mapSkyconIcon[skycon],  QSize(100, 100));
    ui->l_skyIcon->setPixmap(svgIcon);
    ui->l_sky->setText(skyCondition(skycon));
    ui->l_feelTempValue->setText(QString::number(int(feeling)) + "℃");
    ui->l_windValue->setText(windDirect(windDirection) + QString::number(windLevel(windSpeed)) + "级");
    ui->l_humidityValue->setText(QString::number(humidity * 100) + "%");
    ui->l_airValue->setText(chn);

}

void Weather::onNetworkReplyHourlyWeather(QNetworkReply *reply)
{
    if (reply->error()) {
        // qDebug() << "Error:" << reply->errorString();
        return;
    }

    // 读取响应数据
    QByteArray response_data = reply->readAll();

    // 将JSON字符串解析为QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data);

    // 检查解析是否成功
    if (jsonDoc.isNull()) {
        // qDebug() << "Failed to create JSON doc.";
        return;
    }
    if (!jsonDoc.isObject()) {
        // qDebug() << "JSON is not an object.";
        return;
    }

    // 获取根对象
    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject resultObj = jsonObj.value("result").toObject();
    QString forecast = resultObj.value("forecast_keypoint").toString();
    ui->l_forecast->setText(forecast);
}

void Weather::onNetworkReplyMutiWeather(QNetworkReply *reply)
{
    QVector<QString> vecDate;
    QVector<double> vecMaxTemp;
    QVector<double> vecMinTemp;
    QVector<QString> vecSkycon;

    if (reply->error()) {
        // qDebug() << "Error:" << reply->errorString();
        return;
    }

    // 读取响应数据
    QByteArray response_data = reply->readAll();

    // 将JSON字符串解析为QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data);

    // 检查解析是否成功
    if (jsonDoc.isNull()) {
        // qDebug() << "Failed to create JSON doc.";
        return;
    }
    if (!jsonDoc.isObject()) {
        // qDebug() << "JSON is not an object.";
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

    // QPixmap pixmap_tod(mapSkyconIcon[vecSkycon[0]]);
    // ui->l_tod_icon->setPixmap(pixmap_tod.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ui->l_tod_icon->setScaledContents(true); // 确保内容被缩放    QPixmap pixmap_tom(mapSkyconIcon[vecSkycon[1]]);

    // ui->l_tom_icon->setPixmap(pixmap_tom.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ui->l_tom_icon->setScaledContents(true); // 确保内容被缩放

    // QPixmap pixmap_aft(mapSkyconIcon[vecSkycon[2]]);
    // ui->l_aft_icon->setPixmap(pixmap_aft.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ui->l_aft_icon->setScaledContents(true); // 确保内容被缩放

    QPixmap svgTodIcon = loadSvgIcon(mapSkyconIcon[vecSkycon[0]],  QSize(100, 100));
    ui->l_tod_icon->setPixmap(svgTodIcon);

    QPixmap svgTomIcon = loadSvgIcon(mapSkyconIcon[vecSkycon[1]],  QSize(100, 100));
    ui->l_tom_icon->setPixmap(svgTomIcon);

    QPixmap svgAftIcon = loadSvgIcon(mapSkyconIcon[vecSkycon[2]],  QSize(100, 100));
    ui->l_aft_icon->setPixmap(svgAftIcon);

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
