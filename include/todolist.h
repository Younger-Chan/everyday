#ifndef TODOLIST_H
#define TODOLIST_H

#include <QWidget>
// #include <QCalendarWidget>

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

private:
    Ui::todoList *ui;
};

#endif // TODOLIST_H
