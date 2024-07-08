#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QSettings>
#include <QDir>

namespace Ui {
class setting;
}

class setting : public QWidget
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = nullptr);
    ~setting();

private slots:
    void on_pb_apply_clicked();

private:
    Ui::setting *ui;
    QMap<QString, QString> mapSenType;
    QMap<QString, QString> mapStar;

private:

    void initSentence();
    void initStar();
};

#endif // SETTING_H
