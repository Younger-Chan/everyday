#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

// #include "almanac.h"

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
    QNetworkAccessManager *networkCalendar, *networkMoyu, *networkMoyuImg, *networkToday; // , *networkZhichang, *networkZhichangImg
    QString curSelectedDate;
    QString url;

private:

    void initMoyu();
    void loadMoyu(const QString);
    void initToday();

private slots:
    void onNetworkReplyCalendar(QNetworkReply *reply);
    void onNetworkReplyMoyu(QNetworkReply *reply);
    void onNetworkReplyMoyuImg(QNetworkReply *reply);
    void onNetworkReplyToday(QNetworkReply *reply);
    void calendarWidget_selectionChanged();
};

#endif // CALENDAR_H
