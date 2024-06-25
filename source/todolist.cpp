#include "todolist.h"
#include "ui_todolist.h"

todoList::todoList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::todoList)
{
    ui->setupUi(this);
}

todoList::~todoList()
{
    delete ui;
}
