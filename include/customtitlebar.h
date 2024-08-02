#ifndef CUSTOMTITLEBAR_H
#define CUSTOMTITLEBAR_H

#include <QWidget>
#include <QSvgRenderer>
#include <QPainter>

namespace Ui {
class CustomTitleBar;
}

class CustomTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit CustomTitleBar(QWidget *parent = nullptr);
    ~CustomTitleBar();

private:
    Ui::CustomTitleBar *ui;

private:
    QIcon loadSvgIcon(const QString &filePath, const QSize &size);
    void initGui();

signals:
    void minimizeClicked();
    void maximizeClicked();
    void closeClicked();

private slots:
    void onMinimizeClicked();
    void onMaximizeClicked();
    void onCloseClicked();
};

#endif // CUSTOMTITLEBAR_H
