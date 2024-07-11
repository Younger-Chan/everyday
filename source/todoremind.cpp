#include "todoremind.h"
#include "ui_todoremind.h"

todoRemind::todoRemind(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::todoRemind)
{
    ui->setupUi(this);
}

todoRemind::~todoRemind()
{
    delete ui;
}
