#include "todoremind.h"
#include "ui_todoremind.h"

todoRemind::todoRemind(const QString title, const QString notes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::todoRemind)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);
    titleInfo = title;
    notesInfo = notes;
    player = new QMediaPlayer(this);
    initTodoInfo();
}

todoRemind::~todoRemind()
{
    delete ui;
}

void todoRemind::initTodoInfo()
{
    ui->l_title->setText(titleInfo);
    ui->l_notes->setText(notesInfo);

    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);

    // 设置音效文件的路径
    player->setSource(QUrl("qrc:/sound/sound/dingdong.wav"));
    audioOutput->setVolume(50); // 设置音量

    // 播放音效
    player->play();
}

void todoRemind::on_pb_close_clicked()
{
    hide();
}


