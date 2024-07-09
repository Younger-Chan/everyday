#include "setting.h"
#include "ui_setting.h"

setting::setting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting)
{
    ui->setupUi(this);
    initSentence();
    initStar();
    connect(ui->pb_edit, &QPushButton::clicked, this, &setting::handleClicked);
}

setting::~setting()
{
    delete ui;
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
    ui->le_nick->setReadOnly(true);
    ui->le_id->setReadOnly(true);
    ui->le_xq->setReadOnly(true);
    ui->cb_sex->setEnabled(false);
    ui->de_sr->setEnabled(false);
    ui->pb_edit->setText("编辑");
}

void setting::on_pb_tx_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("PNG文件(*.png);;JPEG文件(*.jpg)"));
    if (fileName.isEmpty())
        return;

    // 打开选定的文件
    QFile file(fileName);
    QPixmap pixmap(fileName);
    if(!pixmap.isNull())
        ui->l_tx->setPixmap(pixmap);
}
