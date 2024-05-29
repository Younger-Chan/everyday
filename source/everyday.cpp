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

void EveryDay::on_pb_star_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void EveryDay::on_pb_sub_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void EveryDay::on_pb_me_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

