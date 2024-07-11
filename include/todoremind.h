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
    explicit todoRemind(QWidget *parent = nullptr);
    ~todoRemind();

private:
    Ui::todoRemind *ui;
};

#endif // TODOREMIND_H
