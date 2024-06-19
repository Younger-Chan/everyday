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
    QString key = "PICYEHqRx6tcAL6KkdMjU6TMiJ";
    url = QString("https://api.t1qq.com/api/tool/day/time?key=%1&y=%2&m=%3&d=%4").arg(key, year, month, day);
    // initCalendar(); // year, month, day

    connect(ui->calendarWidget, &QCalendarWidget::selectionChanged, this, &calendar::calendarWidget_selectionChanged);
}

calendar::~calendar()
{
    delete ui;
}

void calendar::calendarWidget_selectionChanged()
{
    ui->l_date->clear();
    ui->l_fes->clear();
    ui->l_jq->clear();
    ui->l_nl->clear();
    ui->l_tgdz->clear();
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
    initMoyu();
    initToday();
    // initZhichang();
}

void calendar::initMoyu()
{
    networkMoyu = new QNetworkAccessManager(this);
    // QString key = "PICYEHqRx6tcAL6KkdMjU6TMiJ";
    QString url_my = "https://api.vvhan.com/api/moyu?type=json";
    QNetworkRequest request_my = QNetworkRequest(QUrl(url_my));
    connect(networkMoyu, &QNetworkAccessManager::finished, this, &calendar::onNetworkReplyMoyu);
    networkMoyu->get(request_my);
}

void calendar::initToday()
{
    networkToday = new QNetworkAccessManager(this);
    QString url_t = "https://api.suyanw.cn/api/today.php";
    QNetworkRequest request_t = QNetworkRequest(QUrl(url_t));
    connect(networkToday, &QNetworkAccessManager::finished, this, &calendar::onNetworkReplyToday);
    networkToday->get(request_t);
}
/*
void calendar::initZhichang()
{
    networkZhichang = new QNetworkAccessManager(this);
    // QString key = "PICYEHqRx6tcAL6KkdMjU6TMiJ";
    QString url_zc = "https://api.vvhan.com/api/zhichang?type=json";
    QNetworkRequest request_zc = QNetworkRequest(QUrl(url_zc));
    connect(networkZhichang, &QNetworkAccessManager::finished, this, &calendar::onNetworkReplyZhichang);
    networkZhichang->get(request_zc);
}*/

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

void calendar::onNetworkReplyMoyu(QNetworkReply *reply)
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
    QString imageUrl = jsonObj["url"].toString(); // 获取第一个图片的 URL
    loadMoyu(imageUrl);
}
/*
void calendar::onNetworkReplyZhichang(QNetworkReply *reply)
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
    QString imageUrl = jsonObj["url"].toString(); // 获取第一个图片的 URL
    loadZhichang(imageUrl);
}*/

void calendar::loadMoyu(const QString imgUrl)
{
    networkMoyuImg = new QNetworkAccessManager(this);
    QNetworkRequest request_my = QNetworkRequest(QUrl(imgUrl));
    connect(networkMoyuImg, &QNetworkAccessManager::finished, this, &calendar::onNetworkReplyMoyuImg);
    networkMoyuImg->get(request_my);
}
/*
void calendar::loadZhichang(const QString imgUrl)
{
    networkZhichangImg = new QNetworkAccessManager(this);
    QNetworkRequest request_my = QNetworkRequest(QUrl(imgUrl));
    connect(networkZhichangImg, &QNetworkAccessManager::finished, this, &calendar::onNetworkReplyZhichangImg);
    networkZhichangImg->get(request_my);
}*/

void calendar::onNetworkReplyMoyuImg(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        // QPixmap scaledPixmap = pixmap.scaled(ui->l_moyu->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // ui->l_moyu->setScaledContents(true);
        // ui->l_moyu->setPixmap(scaledPixmap);
        ui->l_moyu->setPixmap(pixmap);
    } else {
        ui->l_moyu->setText("Failed to load image.");
    }
    reply->deleteLater();
}
/*
void calendar::onNetworkReplyZhichangImg(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        ui->l_zc->setPixmap(pixmap);
    } else {
        ui->l_zc->setText("Failed to load image.");
    }
    reply->deleteLater();
}*/

void calendar::onNetworkReplyToday(QNetworkReply *reply)
{
    QVector<QString> vecToday;
    vecToday.clear();
    ui->lw_today->clear();
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
    QJsonObject dataObj = jsonObj["data"].toObject();
    QJsonArray listArray = dataObj["list"].toArray();
    for(const QJsonValue &value : listArray) {
        QString title = value.toString();
        vecToday << title;
    }
    for(int i = 0; i < vecToday.size(); i++)
    {
        ui->lw_today->addItem(QString::number(i+1) + "." + vecToday[i]);
    }
    reply->deleteLater();
}
