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
    QNetworkAccessManager *networkCalendar, *networkMoyu, *networkMoyuImg, *networkToday;
    QString curSelectedDate;
    QString url;
    QMap<QString, QString> mapStar12;

private:

    void initMoyu();
    void loadMoyu(const QString);
    void initToday();
    void initStar();

private slots:
    void onNetworkReplyCalendar(QNetworkReply *reply);
    void onNetworkReplyMoyu(QNetworkReply *reply);
    void onNetworkReplyMoyuImg(QNetworkReply *reply);
    void onNetworkReplyToday(QNetworkReply *reply);
    // void onNetworkReplyStar(QNetworkReply *reply);
    void calendarWidget_selectionChanged();
};

#endif // CALENDAR_H
