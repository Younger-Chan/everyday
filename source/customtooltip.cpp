#include "customtooltip.h"
#include "ui_customtooltip.h"

CustomToolTip::CustomToolTip(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomToolTip)
{
    ui->setupUi(this);
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

CustomToolTip::~CustomToolTip()
{
    delete ui;
}

void CustomToolTip::showToolTip(const QString &text)
{
    ui->label->setText(text);
    adjustSize();
    show();

    // Auto-hide after 2 seconds
    QTimer::singleShot(2000, this, &CustomToolTip::hide);
}
