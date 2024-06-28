#ifndef TODOLIST_H
#define TODOLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QDir>
#include <QFileInfo>
#include <QDomDocument>
#include <QMessageBox>
#include <QLabel>

#include "flowlayout.h"

namespace Ui {
class todoList;
}

class todoList : public QWidget
{
    Q_OBJECT

public:
    explicit todoList(QWidget *parent = nullptr);
    ~todoList();

private slots:
    void on_pb_firstDay_clicked();

    void on_pb_nextDay_clicked();


    void on_pb_add_clicked();

    void on_pb_today_clicked();

    void on_pb_future_clicked();

    void on_pb_important_clicked();

    void on_pb_all_clicked();

    void on_pb_finish_clicked();

    void on_pb_sure_clicked();


private:
    Ui::todoList *ui;
    FlowLayout *flowLayout;
    QVBoxLayout *layout_info;

private:
    void createXmlFile(const QString &);
    void appendXmlInfo(const QString &);
    int getXmlNextId(const QString &);
    int getTodoCountFromFile(const QString &);
    void updatePage_allWidget(const QString &);
    void clearPage_allWidget();
    void loadTodoXmlFileInfo(const QString &);
    void getTodoXmlFileInfo(const QDomElement &);
    void clearLayout(QLayout *);
};

#endif // TODOLIST_H
