#include "calendar.h"
#include "ui_calendar.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDate>
#include <QDir>

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
    QString url_hl = url;// https://api.t1qq.com/api/tool/day/time?key=PICYEHqRx6tcAL6KkdMjU6TMiJ&y=2024&m=6&d=19   https://api.suyanw.cn/api/huangli.php
    QNetworkRequest request_hl = QNetworkRequest(QUrl(url_hl));
    connect(networkCalendar, &QNetworkAccessManager::finished, this, &calendar::onNetworkReplyCalendar);
    networkCalendar->get(request_hl);
    initMoyu();
    initToday();
    initStar();
    // initZhichang();
}

void calendar::initMoyu()
{
    networkMoyu = new QNetworkAccessManager(this);
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

void calendar::initStar()
{
    mapStar12.clear();
    // 获取应用程序的根目录
    QString rootDir = QCoreApplication::applicationDirPath();

    // 构建相对于根目录的sta.ini文件路径
    QString iniFilePath = QDir(rootDir).filePath("config/star/star12.ini");
    QSettings settings(iniFilePath, QSettings::IniFormat);
    settings.beginGroup("star");
    QString names = settings.value("starEn").toString();
    QStringList namesList = names.split(",");
    QString desc = settings.value("starCh").toString();
    QStringList descList = desc.split(",");
    for(int i = 0; i < namesList.size(); i++)
    {
        mapStar12[namesList[i]] = descList[i];
        ui->cb_star->addItem(descList[i]);
        // qDebug() << namesList[i] << ":" << descList[i];
    }
    settings.endGroup();

    settings.beginGroup("dafult");
    QString name = settings.value("starEn").toString();
    settings.endGroup();
    currentStarEn = name;
    currentTime = "today";
    analysisStar();
    on_pb_today_clicked();
}

void calendar::analysisStar()
{
    ui->cb_star->setCurrentText(mapStar12.value(currentStarEn));
    networkStar = new QNetworkAccessManager(this);
    QString url_s = QString("https://api.vvhan.com/api/horoscope?type=%1&time=%2").arg(currentStarEn, currentTime);
    QNetworkRequest request_s = QNetworkRequest(QUrl(url_s));
    connect(networkStar, &QNetworkAccessManager::finished, this, &calendar::onNetworkReplyStar);
    networkStar->get(request_s);
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

void calendar::loadMoyu(const QString imgUrl)
{
    networkMoyuImg = new QNetworkAccessManager(this);
    QNetworkRequest request_my = QNetworkRequest(QUrl(imgUrl));
    connect(networkMoyuImg, &QNetworkAccessManager::finished, this, &calendar::onNetworkReplyMoyuImg);
    networkMoyuImg->get(request_my);
}

void calendar::onNetworkReplyMoyuImg(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        QPixmap scaledPixmap = pixmap.scaled(ui->l_moyu->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // pixmap()->scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        // ui->l_moyu->setScaledContents(true);
        ui->l_moyu->setPixmap(scaledPixmap);
        // ui->l_moyu->setPixmap(pixmap);
    } else {
        ui->l_moyu->setText("Failed to load image.");
    }
    reply->deleteLater();
}

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

void calendar::onNetworkReplyStar(QNetworkReply *reply)
{
    ui->l_all->clear();
    ui->l_love->clear();
    ui->l_work->clear();
    ui->l_money->clear();
    ui->l_health->clear();
    ui->l_short->clear();
    ui->l_j->clear();
    ui->l_y->clear();
    ui->l_xysz->clear();
    ui->l_xyys->clear();
    ui->l_spxz->clear();

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
    QJsonObject todoObj = dataObj["todo"].toObject();
    QJsonObject fortuneObj = dataObj["fortunetext"].toObject();

    QString yi = todoObj["yi"].toString();
    QString ji = todoObj["ji"].toString();
    QString all = fortuneObj["all"].toString();
    QString love = fortuneObj["love"].toString();
    QString work = fortuneObj["work"].toString();
    QString money = fortuneObj["money"].toString();
    QString health = fortuneObj["health"].toString();
    QString shortComment = dataObj["shortcomment"].toString();
    QString luckynumber = dataObj["luckynumber"].toString();
    QString luckycolor = dataObj["luckycolor"].toString();
    QString luckyconstellation = dataObj["luckyconstellation"].toString();

    ui->l_y->setText(yi);
    ui->l_j->setText(ji);
    ui->l_all->setText(all);
    ui->l_love->setText(love);
    ui->l_work->setText(work);
    ui->l_money->setText(money);
    ui->l_health->setText(health);
    ui->l_short->setText(shortComment);
    ui->l_xysz->setText(luckynumber);
    ui->l_xyys->setText(luckycolor);
    ui->l_spxz->setText(luckyconstellation);

    reply->deleteLater();
}

void calendar::on_cb_star_textActivated(const QString &arg1)
{
    currentStarEn = mapStar12.key(arg1);
    on_pb_today_clicked();
}

void calendar::on_pb_today_clicked()
{
    currentTime = "today";
    analysisStar();
}

void calendar::on_pb_tom_clicked()
{
    currentTime = "nextday";
    analysisStar();
}

void calendar::on_pb_week_clicked()
{
    currentTime = "week";
    analysisStar();
}

void calendar::on_pb_month_clicked()
{
    currentTime = "month";
    analysisStar();
}



