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
    void initCalendar(); // QString, QString, QString
private:
    Ui::calendar *ui;
    QNetworkAccessManager *networkCalendar;
    QString curSelectedDate;
    // int year, month, day;
    QString url;

private:

private slots:
    void onNetworkReplyCalendar(QNetworkReply *reply);
    void on_calendarWidget_selectionChanged();
};

#endif // CALENDAR_H
