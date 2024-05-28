#include "everyday.h"
#include "./ui_everyday.h"

EveryDay::EveryDay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EveryDay)
{
    ui->setupUi(this);
}

EveryDay::~EveryDay()
{
    delete ui;
}
