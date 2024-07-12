#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QPainter>
#include <QPainterPath>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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

    void onNetworkReplyTx(QNetworkReply *reply);
    void onImageDownloaded(QNetworkReply *reply);

    void on_pb_rtx_clicked();

private:
    Ui::setting *ui;
    QMap<QString, QString> mapSenType;
    QMap<QString, QString> mapStar;

    bool clickedFlag = false;

    QString txPhoto;

    QNetworkAccessManager *networkTx, *networkImg;

private:

    //*****编辑按钮*****
    void on_pb_edit_event();
    void on_pb_finish_event();
    void handleClicked();
    //****************

    void initIdentify();
    void initSentence();
    void initStar();
    void initFriendLink();

    QPixmap createRoundedPixmap(const QPixmap &, int);
    void initRandomTx();
};

#endif // SETTING_H
