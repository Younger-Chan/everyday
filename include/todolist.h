#ifndef TODOLIST_H
#define TODOLIST_H

#include <QWidget>

namespace Ui {
class todoList;
}

class todoList : public QWidget
{
    Q_OBJECT

public:
    explicit todoList(QWidget *parent = nullptr);
    ~todoList();

private:
    Ui::todoList *ui;
};

#endif // TODOLIST_H
