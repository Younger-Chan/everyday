#include "todoremind.h"
#include "ui_todoremind.h"

todoRemind::todoRemind(const QString title, const QString notes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::todoRemind)
{
    ui->setupUi(this);
    titleInfo = title;
    notesInfo = notes;
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
}

void todoRemind::on_pb_close_clicked()
{
    reject();
}


