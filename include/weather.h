#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class Weather;
}

class Weather : public QWidget
{
    Q_OBJECT

public:
    explicit Weather(QWidget *parent = nullptr);
    ~Weather();

private:
    Ui::Weather *ui;
    QNetworkAccessManager *networkManager, *networkLocation, *networkWeather, *networkMutiWeather;
    QString longitude, latitude;
    QMap<QString, QString> mapSkyconIcon;

private:
    void initWeather();
    int windLevel(const double);
    QString windDirect(const double);
    QString skyCondition(const QString);

private slots:
    void onNetworkReplyIp(QNetworkReply *reply);
    void onNetworkReplyLocation(QNetworkReply *reply);
    void onNetworkReplyWeather(QNetworkReply *reply);
    void onNetworkReplyMutiWeather(QNetworkReply *reply);
};

#endif // WEATHER_H
