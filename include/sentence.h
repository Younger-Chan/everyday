#ifndef SENTENCE_H
#define SENTENCE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDir>
#include <QSettings>

namespace Ui {
class sentence;
}

class sentence : public QWidget
{
    Q_OBJECT

public:
    explicit sentence(QWidget *parent = nullptr);
    ~sentence();
    void initSen();

private:
    Ui::sentence *ui;
    QNetworkAccessManager *networkManager;

private:
    // void initSen();

private slots:
    void onNetworkReply(QNetworkReply *reply);
};

#endif // SENTENCE_H
