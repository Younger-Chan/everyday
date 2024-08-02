#include "everyday.h"
#include "ui_everyday.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

EveryDay::EveryDay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EveryDay)
{
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    ui->setupUi(this);

    bar = findChild<CustomTitleBar*>("widget");

    connect(bar, &CustomTitleBar::minimizeClicked, this, &EveryDay::showMinimized);
    connect(bar, &CustomTitleBar::maximizeClicked, this, &EveryDay::toggleMaximizeRestore);
    connect(bar, &CustomTitleBar::closeClicked, this, &EveryDay::close);
    connect(bar, &CustomTitleBar::doubleClicked, this, &EveryDay::toggleMaximizeRestore);

    setWindowFlags(Qt::FramelessWindowHint);
    setMenuWidget(bar);
    initGui();
    initSen();
}

EveryDay::~EveryDay()
{
    delete ui;
}

void EveryDay:: toggleMaximizeRestore()
{
    if (isMaximized())
    {
        showNormal();
    }
    else
    {
        showMaximized();
    }
}

void EveryDay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        const QRect &geometry = frameGeometry();
        const QPoint &pos = event->pos();
        const int margin = 5;

        if (pos.x() >= geometry.width() - margin || pos.y() >= geometry.height() - margin)
        {
            m_resizing = true;
            m_dragPosition = event->globalPosition().toPoint();
            m_startGeometry = geometry;
        } else {
            m_resizing = false;
            m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        }
        event->accept();
    }
}

void EveryDay::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint globalPos = event->globalPosition().toPoint();
    const QPoint localPos = event->pos();

    if (m_resizing) {
        resizeWindow(globalPos);
    } else if (event->buttons() & Qt::LeftButton) {
        move(globalPos - m_dragPosition);
    } else {
        updateCursorShape(localPos);
    }
    event->accept();
}

void EveryDay::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_resizing = false;
        event->accept();
    }
}

void EveryDay::resizeWindow(const QPoint &pos)
{
    const int dx = pos.x() - m_dragPosition.x();
    const int dy = pos.y() - m_dragPosition.y();

    setGeometry(m_startGeometry.adjusted(0, 0, dx, dy));
}

void EveryDay::updateCursorShape(const QPoint &pos)
{
    const QRect &geometry = frameGeometry();
    const int margin = 5;
    const int x = pos.x();
    const int y = pos.y();

    if (x >= geometry.width() - margin && y >= geometry.height() - margin)
    {
        setCursor(Qt::SizeBDiagCursor);
    } else if (x >= geometry.width() - margin) {
        setCursor(Qt::SizeHorCursor);
    } else if (y >= geometry.height() - margin) {
        setCursor(Qt::SizeVerCursor);
    } else {
        unsetCursor();
    }
}

void EveryDay::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        toggleMaximizeRestore();
        event->accept();
    }
}

void EveryDay::closeEvent(QCloseEvent *event)
{
    // 忽略关闭事件，即阻止窗口默认的关闭行为
    event->ignore();

    // 隐藏窗口而不是关闭
    hide(); // 或者调用 setVisible(false); 或其他自定义操作
}

void EveryDay::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        // 忽略关闭事件，即阻止窗口默认的关闭行为
        event->ignore();

        // 隐藏窗口而不是关闭
        hide(); // 或者调用 setVisible(false); 或其他自定义操作
    }
}

QIcon EveryDay::loadSvgIcon(const QString &filePath, const QSize &size)
{
    QSvgRenderer svgRenderer(filePath);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    svgRenderer.render(&painter);
    return QIcon(pixmap);
}

void EveryDay::initGui()
{
    QIcon svgGreetIcon = loadSvgIcon(":/tool/tool/message_fill.svg",  QSize(40, 40));
    QIcon svgHotIcon = loadSvgIcon(":/tool/tool/topic_square_fill.svg",  QSize(40, 40));
    QIcon svgTodoIcon = loadSvgIcon(":/tool/tool/menu_box_fill.svg",  QSize(40, 40));
    QIcon svgCalendarIcon = loadSvgIcon(":/tool/tool/calendar_clock_fill.svg",  QSize(40, 40));
    QIcon svgMeIcon = loadSvgIcon(":/tool/tool/apps_me_fill.svg",  QSize(40, 40));
    QIcon svgUpIcon = loadSvgIcon(":/tool/tool/chevron_left_circle_fill.svg",  QSize(40, 40));
    QIcon svgDownIcon = loadSvgIcon(":/tool/tool/chevron_right_circle_fill.svg",  QSize(40, 40));
    ui->pb_greet->setIcon(svgGreetIcon);
    ui->pb_hot->setIcon(svgHotIcon);
    ui->pb_todo->setIcon(svgTodoIcon);
    ui->pb_sub->setIcon(svgCalendarIcon);
    ui->pb_me->setIcon(svgMeIcon);
    ui->pb_up->setIcon(svgUpIcon);
    ui->pb_down->setIcon(svgDownIcon);
}

void EveryDay::initSen()
{
    // 获取应用程序的根目录
    QString rootDir = QCoreApplication::applicationDirPath();

    // 构建相对于根目录的sta.ini文件路径
    QString iniFilePath = QDir(rootDir).filePath("config/sentence/sentence.ini");
    QSettings settings(iniFilePath, QSettings::IniFormat);
    settings.beginGroup("dafult");
    QString name = settings.value("sentenceEn").toString();
    // QStringList namesList = names.split(",");
    // QString desc = settings.value("sentenceCh").toString();
    // QStringList descList = desc.split(",");

    settings.endGroup();


    ui->stackedWidget->setCurrentIndex(0);
    networkManager = new QNetworkAccessManager(this);

    QString url_base = "https://v1.hitokoto.cn/";
    QString url_type = QString("?c=%1").arg(name);
    QString url_encode = "&encode=json";
    QString url_sen = url_base + url_type + url_encode;
    // QString url_sen = "https://v1.hitokoto.cn/?c=i&encode=json";// https://v1.hitokoto.cn/?c=f&encode=text   https://api.a20safe.com/api.php?api=6&key=6e64858a2dec587348d3ed9adaa0a66b&type=i
    QNetworkRequest request_sen = QNetworkRequest(QUrl(url_sen));
    connect(networkManager, &QNetworkAccessManager::finished, this, &EveryDay::onNetworkReply);

    networkManager->get(request_sen);
}

void EveryDay::on_pb_greet_clicked()
{
    initSen();
    ui->w_weather->initWeather();
}

void EveryDay::onNetworkReply(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

        if(!jsonDoc.isNull())
        {
            QJsonObject jsonObj = jsonDoc.object();
            QString hitokoto = jsonObj["hitokoto"].toString();
            QString from = jsonObj["from"].toString();
            QString who = jsonObj["from_who"].toString();
            QString htmlText;
            htmlText += QString("<p>%1</p>").arg(hitokoto);
            htmlText += QString("<p>%1•《%2》</p>").arg(who, from);
            ui->l_greet->setText(htmlText);
        }
        else
        {
            ui->l_greet->setText("Error parsing JSON");
        }
    }
    else
    {
        ui->l_greet->setText("Network error: " + reply->errorString());
    }
    reply->deleteLater();
}

void EveryDay::on_pb_hot_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_hot->setCurrentIndex(0);
    ui->page_weibo->initWeibo();
    ui->page_baidu->initBaidu();
    ui->page_douyin->initDouyin();
    ui->page_zhihu->initZhihu();
    ui->page_blbl->initBlbl();
    ui->page_jian->initJian();
    ui->page_wyy->initWyy();
}

void EveryDay::on_pb_todo_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void EveryDay::on_pb_sub_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->w_calendar->initCalendar();
}

void EveryDay::on_pb_me_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void EveryDay::on_pb_down_clicked()
{
    int currentIndex = ui->stackedWidget_hot->currentIndex();
    int nextIndex = (currentIndex + 1) % ui->stackedWidget_hot->count();
    ui->stackedWidget_hot->setCurrentIndex(nextIndex);
}

void EveryDay::on_pb_up_clicked()
{
    int currentIndex = ui->stackedWidget_hot->currentIndex();
    int prevIndex = (currentIndex - 1 + ui->stackedWidget_hot->count()) % ui->stackedWidget_hot->count();
    ui->stackedWidget_hot->setCurrentIndex(prevIndex);
}



