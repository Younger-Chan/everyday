#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>

namespace Ui {
class calendar;
}

class calendar : public QWidget
{
    Q_OBJECT

public:
    explicit calendar(QWidget *parent = nullptr);
    ~calendar();
    void initCalendar();

private:
    Ui::calendar *ui;
    QNetworkAccessManager *networkCalendar, *networkMoyu, *networkMoyuImg, *networkToday, *networkStar;
    QString curSelectedDate;
    QString url;
    QMap<QString, QString> mapStar12;
    QString currentStarEn, currentTime;

private:

    void initMoyu();
    void loadMoyu(const QString);
    void initToday();
    void initStar();
    void analysisStar();

private slots:
    void onNetworkReplyCalendar(QNetworkReply *reply);
    void onNetworkReplyMoyu(QNetworkReply *reply);
    void onNetworkReplyMoyuImg(QNetworkReply *reply);
    void onNetworkReplyToday(QNetworkReply *reply);
    void onNetworkReplyStar(QNetworkReply *reply);
    void calendarWidget_selectionChanged();
    void on_cb_star_textActivated(const QString &arg1);
    void on_pb_today_clicked();
    void on_pb_tom_clicked();
    void on_pb_week_clicked();
    void on_pb_month_clicked();
};

#endif // CALENDAR_H
