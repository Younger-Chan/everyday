#include "bilibili.h"
#include "ui_bilibili.h"

bilibili::bilibili(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::bilibili)
{
    ui->setupUi(this);
}

bilibili::~bilibili()
{
    delete ui;
}
