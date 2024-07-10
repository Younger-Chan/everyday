#include "setting.h"
#include "ui_setting.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

setting::setting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting)
{
    ui->setupUi(this);
    initIdentify();
    initSentence();
    initStar();
    connect(ui->pb_edit, &QPushButton::clicked, this, &setting::handleClicked);
}

setting::~setting()
{
    delete ui;
}

void setting::initIdentify()
{
    // 获取应用程序的根目录
    QString rootDir = QCoreApplication::applicationDirPath();

    // 构建相对于根目录的sta.ini文件路径
    QString iniFilePath = QDir(rootDir).filePath("config/setting/identify.ini");
    QSettings settings(iniFilePath, QSettings::IniFormat);
    settings.beginGroup("identify");
    QString photo = settings.value("photo").toString();
    QString nick = settings.value("nick").toString();
    qDebug() << nick;
    QString id = settings.value("id").toString();
    QString sex = settings.value("sex").toString();
    QString sr = settings.value("birthday").toString();
    QDate date = QDate::fromString(sr, "yyyy-MM-dd");
    QString xq = settings.value("interest").toString();

    networkImg = new QNetworkAccessManager(this);
    connect(networkImg, &QNetworkAccessManager::finished, this, &setting::onImageDownloaded);
    QUrl imageUrl(photo);
    QNetworkRequest request(imageUrl);
    networkImg->get(request);
    txPhoto = photo;
    ui->le_nick->setText(nick);
    ui->le_id->setText(id);
    ui->cb_sex->setCurrentText(sex);
    ui->de_sr->setDate(date);
    ui->le_xq->setText(xq);
    settings.endGroup();

}

void setting::initSentence()
{
    mapSenType.clear();
    // 获取应用程序的根目录
    QString rootDir = QCoreApplication::applicationDirPath();

    // 构建相对于根目录的sta.ini文件路径
    QString iniFilePath = QDir(rootDir).filePath("config/sentence/sentence.ini");
    QSettings settings(iniFilePath, QSettings::IniFormat);
    settings.beginGroup("sentence");
    QString names = settings.value("sentenceEn").toString();
    QStringList namesList = names.split(",");
    QString desc = settings.value("sentenceCh").toString();
    QStringList descList = desc.split(",");
    for(int i = 0; i < namesList.size(); i++)
    {
        mapSenType[namesList[i]] = descList[i];
        ui->cb_senType->addItem(descList[i]);
    }
    settings.endGroup();

    settings.beginGroup("dafult");
    QString dafultSen = settings.value("sentenceCh").toString();
    settings.endGroup();
    ui->cb_senType->setCurrentText(dafultSen);
}

void setting::initStar()
{
    mapStar.clear();
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
        mapStar[namesList[i]] = descList[i];
        ui->cb_defaultStar->addItem(descList[i]);
    }
    settings.endGroup();

    settings.beginGroup("dafult");
    QString dafultStar = settings.value("starCh").toString();
    settings.endGroup();
    ui->cb_defaultStar->setCurrentText(dafultStar);
}

void setting::on_pb_apply_clicked()
{
    QString sentenceDir = QCoreApplication::applicationDirPath();
    // 构建相对于根目录的sta.ini文件路径
    QString iniFilePathSen = QDir(sentenceDir).filePath("config/sentence/sentence.ini");
    QSettings settings_sen(iniFilePathSen, QSettings::IniFormat);
    settings_sen.beginGroup("dafult");
    QString typeSenEn = mapSenType.key(ui->cb_senType->currentText());
    QString typeSenCh = ui->cb_senType->currentText();
    settings_sen.setValue("sentenceEn", typeSenEn);
    settings_sen.setValue("sentenceCh", typeSenCh);
    settings_sen.endGroup();

    QString starDir = QCoreApplication::applicationDirPath();
    // 构建相对于根目录的sta.ini文件路径
    QString iniFilePathStar = QDir(starDir).filePath("config/star/star12.ini");
    QSettings settings_star(iniFilePathStar, QSettings::IniFormat);
    settings_star.beginGroup("dafult");
    QString typeStarEn = mapStar.key(ui->cb_defaultStar->currentText());
    QString typeStarCh = ui->cb_defaultStar->currentText();
    settings_star.setValue("starEn", typeStarEn);
    settings_star.setValue("starCh", typeStarCh);
    settings_star.endGroup();
}

void setting::handleClicked()
{
    if(!clickedFlag)
        on_pb_edit_event();
    else
    {
        on_pb_finish_event();
    }
    clickedFlag = !clickedFlag;
}

void setting::on_pb_edit_event()
{
    ui->pb_tx->setEnabled(true);
    ui->pb_rtx->setEnabled(true);
    ui->le_nick->setReadOnly(false);
    ui->le_id->setReadOnly(false);
    ui->le_xq->setReadOnly(false);
    ui->cb_sex->setEnabled(true);
    ui->de_sr->setEnabled(true);
    ui->pb_edit->setText("完成");
}

void setting::on_pb_finish_event()
{
    ui->pb_tx->setEnabled(false);
    ui->pb_rtx->setEnabled(false);
    ui->le_nick->setReadOnly(true);
    ui->le_id->setReadOnly(true);
    ui->le_xq->setReadOnly(true);
    ui->cb_sex->setEnabled(false);
    ui->de_sr->setEnabled(false);
    ui->pb_edit->setText("编辑");

    QString settingDir = QCoreApplication::applicationDirPath();
    // 构建相对于根目录的sta.ini文件路径
    QString iniFilePathIdentify = QDir(settingDir).filePath("config/setting/identify.ini");
    QSettings settings_identify(iniFilePathIdentify, QSettings::IniFormat);
    settings_identify.beginGroup("identify");
    settings_identify.setValue("photo", txPhoto);
    QString nick = ui->le_nick->text();
    QString id = ui->le_id->text();
    QString sex = ui->cb_sex->currentText();
    QDate sr_D = ui->de_sr->date();
    QString sr = sr_D.toString("yyyy-MM-dd");
    QString xq = ui->le_xq->text();
    settings_identify.setValue("nick", nick);
    settings_identify.setValue("id", id);
    settings_identify.setValue("sex", sex);
    settings_identify.setValue("birthday", sr);
    settings_identify.setValue("interest", xq);
    settings_identify.endGroup();
}

void setting::on_pb_tx_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("PNG文件(*.png);;JPEG文件(*.jpg)"));
    if (fileName.isEmpty())
        return;

    txPhoto = fileName;
    // 打开选定的文件
    QFile file(fileName);
    // QPixmap roundedPixmap = createRoundedPixmap(originalPixmap, originalPixmap.width() / 2);
    QPixmap pixmap = createRoundedPixmap(fileName, 50);
    if(!pixmap.isNull())
        ui->l_tx->setPixmap(pixmap);
}

QPixmap setting::createRoundedPixmap(const QPixmap &src, int radius)
{
    int size = qMin(src.width(), src.height());
    QPixmap roundedPixmap(size, size);
    roundedPixmap.fill(Qt::transparent);

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    painter.setClipPath(path);

    painter.drawPixmap(0, 0, src);

    return roundedPixmap;
}

void setting::on_pb_rtx_clicked()
{
    initRandomTx();
}

void setting::initRandomTx()
{
    networkTx = new QNetworkAccessManager(this);
    QString url_tx = "https://api.suyanw.cn/api/sjtx.php?return=json";
    QNetworkRequest request_hot = QNetworkRequest(QUrl(url_tx));
    connect(networkTx, &QNetworkAccessManager::finished, this, &setting::onNetworkReplyTx);
    networkTx->get(request_hot);
}

void setting::onNetworkReplyTx(QNetworkReply *reply)
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

    QString pic = jsonObj.value("imgurl").toString();
    txPhoto = pic;

    reply->deleteLater();

    networkImg = new QNetworkAccessManager(this);
    connect(networkImg, &QNetworkAccessManager::finished, this, &setting::onImageDownloaded);
    QUrl imageUrl(pic);
    QNetworkRequest request(imageUrl);
    networkImg->get(request);
}

void setting::onImageDownloaded(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray imageData = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(imageData);
        QPixmap scaledPixmap = createRoundedPixmap(pixmap, 50);
        if(!scaledPixmap.isNull())
            ui->l_tx->setPixmap(scaledPixmap);
    }
    else
    {
        qWarning() << "Error downloading image:" << reply->errorString();
    }
    reply->deleteLater();
}
