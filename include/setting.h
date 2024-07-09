#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileDialog>

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

    void on_pb_tx_clicked();

    //*****编辑按钮*****
    void on_pb_edit_event();
    void on_pb_finish_event();
    void handleClicked();
    //****************

private:
    Ui::setting *ui;
    QMap<QString, QString> mapSenType;
    QMap<QString, QString> mapStar;

    bool clickedFlag = false;

private:

    void initSentence();
    void initStar();
};

#endif // SETTING_H
