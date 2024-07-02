#ifndef TODOLIST_H
#define TODOLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QDir>
#include <QFileInfo>
#include <QDomDocument>
#include <QMessageBox>
#include <QLabel>
#include <QScrollArea>

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
    FlowLayout *flowLayout, *flowLayoutCur, *flowLayoutFt, *flowLayoutZy, *flowLayoutFs;
    QVBoxLayout *layout_AllInfo, *layout_curInfo, *layoutFtInfo, *layoutZyInfo, *layoutFsInfo;
    QWidget *centralWidgetAll, *centralWidgetCur, *centralWidgetFt, *centralWidgetZy, *centralWidgetFs;
    QScrollArea *scrollAreaAll, *scrollAreaCur, *scrollAreaFt, *scrollAreaZy, *scrollAreaFs;
    QVBoxLayout *vlayoutAll, *vlayoutCur, *vlayoutFt, *vlayoutZy, *vlayoutFs;

private:

    void initLayout();
    void createXmlFile(const QString &);
    void appendXmlInfo(const QString &);
    int getXmlNextId(const QString &);
    int getTodoCountFromFile(const QString &);
    void updatePage_futureWidget(const QString &);
    void updatePage_curWidget(const QString &);
    void updatePage_allWidget(const QString &);
    void updatePage_zyWidget(const QString &);

    void clearPage_ftWidget();
    void clearPage_curWidget();
    void clearPage_allWidget();
    void clearPage_zyWidget();

    void loadTodoXmlFileFtInfo(const QString &);
    void loadTodoXmlFileCurInfo(const QString &);
    void loadTodoXmlFileAllInfo(const QString &);
    void loadTodoXmlFileZyInfo(const QString &);

    void getTodoXmlFileFtInfo(const QDomElement &);
    void getTodoXmlFileCurInfo(const QDomElement &);
    void getTodoXmlFileAllInfo(const QDomElement &);
    void getTodoXmlFileZyInfo(const QDomElement &);
    void clearLayout(QLayout *);
};

#endif // TODOLIST_H
