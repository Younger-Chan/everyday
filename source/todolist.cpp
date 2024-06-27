#include "todolist.h"
#include "ui_todolist.h"
#include <QScrollArea>
#include <QLabel>

todoList::todoList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::todoList)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
    QTime curTime = QTime::currentTime();
    ui->timeEdit->setTime(curTime);

}

todoList::~todoList()
{
    delete ui;
}

void todoList::on_pb_firstDay_clicked()
{
    QDate curDate = QDate::currentDate();
    ui->dateEdit->setDate(curDate);
}

void todoList::on_pb_nextDay_clicked()
{
    QDate curDate = QDate::currentDate();
    QDate nextDay = curDate.addDays(1);
    ui->dateEdit->setDate(nextDay);
}

void todoList::on_pb_add_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void todoList::on_pb_today_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void todoList::on_pb_future_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void todoList::on_pb_important_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void todoList::on_pb_all_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void todoList::on_pb_finish_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void todoList::on_pb_sure_clicked()
{
    QString rootDir = QCoreApplication::applicationDirPath();

    // 构建相对于根目录的sta.ini文件路径
    QString xmlFilePath = QDir(rootDir).filePath("config/todo/todo.xml");
    QFileInfo file(xmlFilePath);
    if(!file.exists())
    {
        createXmlFile(xmlFilePath);
    }
}

void todoList::createXmlFile(const QString &file)
{

}
