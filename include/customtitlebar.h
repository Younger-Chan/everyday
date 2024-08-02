#ifndef CUSTOMTITLEBAR_H
#define CUSTOMTITLEBAR_H

#include <QWidget>
#include <QSvgRenderer>
#include <QPainter>
#include <QMouseEvent>

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
    bool maxminFlag = false;

    QPoint m_dragPosition;

private:
    QIcon loadSvgIcon(const QString &filePath, const QSize &size);
    void initGui();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void minimizeClicked();
    void maximizeClicked();
    void closeClicked();
    void doubleClicked();

private slots:
    void onMinimizeClicked();
    void onMaximizeClicked();
    void onCloseClicked();
    void handleClicked();
};

#endif // CUSTOMTITLEBAR_H
