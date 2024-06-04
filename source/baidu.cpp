#include "baidu.h"
#include "ui_baidu.h"

baidu::baidu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::baidu)
{
    ui->setupUi(this);
}

baidu::~baidu()
{
    delete ui;
}
