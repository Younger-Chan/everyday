#ifndef EVERYDAY_H
#define EVERYDAY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class EveryDay;
}
QT_END_NAMESPACE

class EveryDay : public QMainWindow
{
    Q_OBJECT

public:
    EveryDay(QWidget *parent = nullptr);
    ~EveryDay();

private:
    Ui::EveryDay *ui;
};
#endif // EVERYDAY_H
