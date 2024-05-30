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
    QNetworkAccessManager *networkManager, *networkLocation, *networkWeather;
    QString longitude, latitude;

private:
    int windLevel(const double);
    QString windDirect(const double);

private slots:
    void onFinished(QNetworkReply *reply);
    void onNetworkReplyLocation(QNetworkReply *reply);
    void onNetworkReplyWeather(QNetworkReply *reply);
};

#endif // WEATHER_H
