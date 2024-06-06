#include "almanac.h"
#include "ui_almanac.h"

almanac::almanac(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::almanac)
{
    ui->setupUi(this);
}

almanac::~almanac()
{
    delete ui;
}
