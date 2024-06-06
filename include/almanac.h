#ifndef ALMANAC_H
#define ALMANAC_H

#include <QWidget>

namespace Ui {
class almanac;
}

class almanac : public QWidget
{
    Q_OBJECT

public:
    explicit almanac(QWidget *parent = nullptr);
    ~almanac();

private:
    Ui::almanac *ui;
};

#endif // ALMANAC_H
