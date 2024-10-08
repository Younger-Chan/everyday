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
#include <QTimer>
#include <QMenu>
#include <QListWidget>
#include <QSvgRenderer>
#include <QPainter>

#include "customtooltip.h"

// #include "flowlayout.h"

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

    void checkDateTime();
    void showReminder();

    void on_actionRemoveTodo_triggered();

private:
    Ui::todoList *ui;
    // FlowLayout *flowLayout, *flowLayoutCur, *flowLayoutFt, *flowLayoutZy, *flowLayoutFs;
    // QVBoxLayout *layout_allInfo, *layout_curInfo, *layout_ftInfo, *layout_zyInfo, *layout_fsInfo;
    // QWidget *centralWidgetAll, *centralWidgetCur, *centralWidgetFt, *centralWidgetZy, *centralWidgetFs;
    // QScrollArea *scrollAreaAll, *scrollAreaCur, *scrollAreaFt, *scrollAreaZy, *scrollAreaFs;
    // QVBoxLayout *vlayoutAll, *vlayoutCur, *vlayoutFt, *vlayoutZy, *vlayoutFs;

    QTimer *timer;
    QDateTime targetDatetime;

    QString targetTitle, targetNotes;
    QIcon loadSvgIcon(const QString &filePath, const QSize &size);
    CustomToolTip *tooltip;

    // QString listWidgetObjectName;
    // QListWidget *curList;

    // todoRemind *remind;

private:

    // void initLayout();
    void createXmlFile(const QString &);
    void appendXmlInfo(const QString &);
    int getXmlNextId(const QString &);
    int getTodoCountFromFile(const QString &);
    void updatePage_futureWidget(const QString &);
    void updatePage_curWidget(const QString &);
    void updatePage_allWidget(const QString &);
    void updatePage_zyWidget(const QString &);
    void updatePage_fsWidget(const QString &);

    // void clearPage_ftWidget();
    // void clearPage_curWidget();
    // void clearPage_allWidget();
    // void clearPage_zyWidget();
    // void clearPage_fsWidget();

    void loadTodoXmlFileFtInfo(const QString &);
    void loadTodoXmlFileCurInfo(const QString &);
    void loadTodoXmlFileAllInfo(const QString &);
    void loadTodoXmlFileZyInfo(const QString &);
    void loadTodoXmlFileFsInfo(const QString &);

    void getTodoXmlFileFtInfo(const QDomElement &);
    void getTodoXmlFileCurInfo(const QDomElement &);
    void getTodoXmlFileAllInfo(const QDomElement &);
    void getTodoXmlFileZyInfo(const QDomElement &);
    void getTodoXmlFileFsInfo(const QDomElement &);
    // void clearLayout(QLayout *);

    void contextMenuEvent(QContextMenuEvent *event);

};

#endif // TODOLIST_H
