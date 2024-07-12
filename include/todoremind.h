#ifndef TODOREMIND_H
#define TODOREMIND_H

#include <QDialog>

namespace Ui {
class todoRemind;
}

class todoRemind : public QDialog
{
    Q_OBJECT

public:
    explicit todoRemind(const QString title, const QString notes, QWidget *parent = nullptr);
    ~todoRemind();

private slots:
    void on_pb_close_clicked();

private:
    Ui::todoRemind *ui;
    QString titleInfo, notesInfo;

private:
    void initTodoInfo();
};

#endif // TODOREMIND_H
