#ifndef BAIDU_H
#define BAIDU_H

#include <QWidget>

namespace Ui {
class baidu;
}

class baidu : public QWidget
{
    Q_OBJECT

public:
    explicit baidu(QWidget *parent = nullptr);
    ~baidu();

private:
    Ui::baidu *ui;
};

#endif // BAIDU_H
