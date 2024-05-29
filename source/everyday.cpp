#include "everyday.h"
#include "./ui_everyday.h"

EveryDay::EveryDay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EveryDay)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

EveryDay::~EveryDay()
{
    delete ui;
}

void EveryDay::on_pb_greet_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void EveryDay::on_pb_hot_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

