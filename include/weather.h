#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSvgRenderer>
#include <QPainter>

namespace Ui {
class Weather;
}

class Weather : public QWidget
{
    Q_OBJECT

public:
    explicit Weather(QWidget *parent = nullptr);
    ~Weather();
    void initWeather();

private:
    Ui::Weather *ui;
    QNetworkAccessManager *networkManager, *networkLocation, *networkWeather, *networkMutiWeather;
    QString longitude, latitude;
    QMap<QString, QString> mapSkyconIcon;
private:
    int windLevel(const double);
    QString windDirect(const double);
    QString skyCondition(const QString);
    QPixmap loadSvgIcon(const QString &filePath, const QSize &size);


private slots:
    void onNetworkReplyIp(QNetworkReply *reply);
    void onNetworkReplyLocation(QNetworkReply *reply);
    void onNetworkReplyWeather(QNetworkReply *reply);
    void onNetworkReplyMutiWeather(QNetworkReply *reply);
};

#endif // WEATHER_H
