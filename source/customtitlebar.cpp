#include "customtitlebar.h"
#include "ui_customtitlebar.h"

CustomTitleBar::CustomTitleBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomTitleBar)
{
    ui->setupUi(this);
    initGui();
    // 连接按钮信号到槽函数
    connect(ui->pb_min, &QPushButton::clicked, this, &CustomTitleBar::onMinimizeClicked);
    connect(ui->pb_max, &QPushButton::clicked, this, &CustomTitleBar::onMaximizeClicked);
    connect(ui->pb_close, &QPushButton::clicked, this, &CustomTitleBar::onCloseClicked);
}

CustomTitleBar::~CustomTitleBar()
{
    delete ui;
}

QIcon CustomTitleBar::loadSvgIcon(const QString &filePath, const QSize &size)
{
    QSvgRenderer svgRenderer(filePath);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    svgRenderer.render(&painter);
    return QIcon(pixmap);
}

void CustomTitleBar::initGui()
{
    QIcon svgMinIcon = loadSvgIcon(":/tool/tool/subtract_fill.svg",  QSize(30, 30));
    QIcon svgMaxIcon = loadSvgIcon(":/tool/tool/maximize_fill.svg",  QSize(30, 30));
    QIcon svgCloseIcon = loadSvgIcon(":/tool/tool/close_fill.svg",  QSize(30, 30));
    ui->pb_min->setIcon(svgMinIcon);
    ui->pb_max->setIcon(svgMaxIcon);
    ui->pb_close->setIcon(svgCloseIcon);
}

void CustomTitleBar::onMinimizeClicked()
{
    emit minimizeClicked();
}

void CustomTitleBar::onMaximizeClicked()
{
    QIcon svgMaxIcon = loadSvgIcon(":/tool/tool/minimize_fill.svg",  QSize(30, 30));
    ui->pb_max->setIcon(svgMaxIcon);
    emit maximizeClicked();
}

void CustomTitleBar::onCloseClicked()
{
    emit closeClicked();
}
