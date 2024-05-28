/********************************************************************************
** Form generated from reading UI file 'everyday.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVERYDAY_H
#define UI_EVERYDAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EveryDay
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *EveryDay)
    {
        if (EveryDay->objectName().isEmpty())
            EveryDay->setObjectName("EveryDay");
        EveryDay->resize(800, 600);
        centralwidget = new QWidget(EveryDay);
        centralwidget->setObjectName("centralwidget");
        EveryDay->setCentralWidget(centralwidget);
        menubar = new QMenuBar(EveryDay);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        EveryDay->setMenuBar(menubar);
        statusbar = new QStatusBar(EveryDay);
        statusbar->setObjectName("statusbar");
        EveryDay->setStatusBar(statusbar);

        retranslateUi(EveryDay);

        QMetaObject::connectSlotsByName(EveryDay);
    } // setupUi

    void retranslateUi(QMainWindow *EveryDay)
    {
        EveryDay->setWindowTitle(QCoreApplication::translate("EveryDay", "EveryDay", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EveryDay: public Ui_EveryDay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVERYDAY_H
