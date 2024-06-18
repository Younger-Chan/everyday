#include "calendar.h"
#include "ui_calendar.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDate>

calendar::calendar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::calendar)
{
    ui->setupUi(this);

    QDate date = QDate::currentDate();
    QString dateStr = date.toString("yyyy/MM/dd");
    ui->l_date->setText(dateStr);
    QString year = QString::number(date.year());
    QString month = QString::number(date.month());
    QString day = QString::number(date.day());
    // qDebug() << year << " " << month << " " << day;
    QString key = "PICYEHqRx6tcAL6KkdMjU6TMiJ";
    url = QString("https://api.t1qq.com/api/tool/day/time?key=%1&y=%2&m=%3&d=%4").arg(key, year, month, day);
    initCalendar(); // year, month, day
}

calendar::~calendar()
{
    delete ui;
}

void calendar::on_calendarWidget_selectionChanged()
{
    QDate date = ui->calendarWidget->selectedDate();
    QString dateStr = date.toString("yyyy/MM/dd");
    ui->l_date->setText(dateStr);
    QString year = QString::number(date.year());
    QString month = QString::number(date.month());
    QString day = QString::number(date.day());
    QString key = "PICYEHqRx6tcAL6KkdMjU6TMiJ";
    url = QString("https://api.t1qq.com/api/tool/day/time?key=%1&y=%2&m=%3&d=%4").arg(key, year, month, day);
    initCalendar(); // year, month, day

}
void calendar::initCalendar() // QString y, QString m, QString d
{
    networkCalendar = new QNetworkAccessManager(this);
    // QString key = "PICYEHqRx6tcAL6KkdMjU6TMiJ";
    QString url_hl = url;// https://api.t1qq.com/api/tool/day/time?key=PICYEHqRx6tcAL6KkdMjU6TMiJ&y=2024&m=6&d=19   https://api.suyanw.cn/api/huangli.php
    QNetworkRequest request_hl = QNetworkRequest(QUrl(url_hl));
    connect(networkCalendar, &QNetworkAccessManager::finished, this, &calendar::onNetworkReplyCalendar);
    networkCalendar->get(request_hl);
}

void calendar::onNetworkReplyCalendar(QNetworkReply *reply)
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
    QJsonObject jsonObj = jsonDoc.object();

    if (jsonObj["code"].toInt() == 200)
    {
        QString zodiac = jsonObj["zodiac"].toString();
        QString gzYear = jsonObj["GanZhiYear"].toString();
        QString gzMonth = jsonObj["GanZhiMonth"].toString();
        QString gzDay = jsonObj["GanZhiDay"].toString();
        QString tgdz = gzYear + "年" + gzMonth + "月" + gzDay + "日";
        QString jq = jsonObj["term"].toString();

        QJsonObject dataObj = jsonObj["data"].toObject();
        QString nl = dataObj["lunar"].toString();

        // qDebug() << "---";
        QJsonObject fesObj = dataObj["festival"].toObject();
        QJsonArray fesArray = fesObj["solarAnniversary"].toArray();
        for(const QJsonValue &value : fesArray)
        {
            // QJsonObject solarObj = value.toObject();
            QString fes = value.toString();
            ui->l_fes->setText(fes);
        }

        ui->l_nl->setText(nl + " " + zodiac);
        ui->l_tgdz->setText(tgdz);
        ui->l_jq->setText(jq);
    }else {
        qDebug() << "错误:" << jsonObj["msg"].toString();
    }
    reply->deleteLater();
}

