#ifndef CUSTOMTOOLTIP_H
#define CUSTOMTOOLTIP_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class CustomToolTip;
}

class CustomToolTip : public QWidget
{
    Q_OBJECT

public:
    explicit CustomToolTip(QWidget *parent = nullptr);
    ~CustomToolTip();
    void showToolTip(const QString &);

private:
    Ui::CustomToolTip *ui;
};

#endif // CUSTOMTOOLTIP_H
