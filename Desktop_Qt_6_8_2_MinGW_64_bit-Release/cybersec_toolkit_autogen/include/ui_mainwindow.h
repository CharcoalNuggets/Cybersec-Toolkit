/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *fileScanButton;
    QPushButton *stegButton;
    QStackedWidget *stackedWidget;
    QWidget *page_1;
    QPushButton *imageButton;
    QTextEdit *messageTextEdit;
    QPushButton *encodeButton;
    QPushButton *decodeButton;
    QLabel *imageLabel;
    QLabel *decodeLabel;
    QWidget *page_2;
    QPushButton *fileButton;
    QPushButton *hashButton;
    QPushButton *compareButton;
    QMenuBar *menubar;
    QMenu *menuCybersec_Toolkit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        fileScanButton = new QPushButton(centralwidget);
        fileScanButton->setObjectName("fileScanButton");
        fileScanButton->setGeometry(QRect(10, 40, 181, 41));
        fileScanButton->setMaximumSize(QSize(181, 51));
        stegButton = new QPushButton(centralwidget);
        stegButton->setObjectName("stegButton");
        stegButton->setGeometry(QRect(10, 100, 181, 41));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(210, 20, 571, 521));
        page_1 = new QWidget();
        page_1->setObjectName("page_1");
        imageButton = new QPushButton(page_1);
        imageButton->setObjectName("imageButton");
        imageButton->setGeometry(QRect(390, 140, 80, 24));
        messageTextEdit = new QTextEdit(page_1);
        messageTextEdit->setObjectName("messageTextEdit");
        messageTextEdit->setGeometry(QRect(70, 300, 281, 101));
        encodeButton = new QPushButton(page_1);
        encodeButton->setObjectName("encodeButton");
        encodeButton->setGeometry(QRect(390, 340, 80, 24));
        decodeButton = new QPushButton(page_1);
        decodeButton->setObjectName("decodeButton");
        decodeButton->setGeometry(QRect(390, 450, 80, 24));
        imageLabel = new QLabel(page_1);
        imageLabel->setObjectName("imageLabel");
        imageLabel->setGeometry(QRect(70, 20, 281, 256));
        imageLabel->setMinimumSize(QSize(281, 0));
        decodeLabel = new QLabel(page_1);
        decodeLabel->setObjectName("decodeLabel");
        decodeLabel->setGeometry(QRect(70, 420, 281, 91));
        decodeLabel->setWordWrap(true);
        stackedWidget->addWidget(page_1);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        fileButton = new QPushButton(page_2);
        fileButton->setObjectName("fileButton");
        fileButton->setGeometry(QRect(170, 260, 80, 24));
        hashButton = new QPushButton(page_2);
        hashButton->setObjectName("hashButton");
        hashButton->setGeometry(QRect(340, 220, 91, 41));
        compareButton = new QPushButton(page_2);
        compareButton->setObjectName("compareButton");
        compareButton->setGeometry(QRect(340, 290, 91, 41));
        stackedWidget->addWidget(page_2);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuCybersec_Toolkit = new QMenu(menubar);
        menuCybersec_Toolkit->setObjectName("menuCybersec_Toolkit");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuCybersec_Toolkit->menuAction());

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        fileScanButton->setText(QCoreApplication::translate("MainWindow", "File Scanner", nullptr));
        stegButton->setText(QCoreApplication::translate("MainWindow", "Steganography", nullptr));
        imageButton->setText(QCoreApplication::translate("MainWindow", "Load Image", nullptr));
        encodeButton->setText(QCoreApplication::translate("MainWindow", "Encode", nullptr));
        decodeButton->setText(QCoreApplication::translate("MainWindow", "Decode", nullptr));
        imageLabel->setText(QString());
        decodeLabel->setText(QString());
        fileButton->setText(QCoreApplication::translate("MainWindow", "Select File", nullptr));
        hashButton->setText(QCoreApplication::translate("MainWindow", "Create Hash", nullptr));
        compareButton->setText(QCoreApplication::translate("MainWindow", "Compare File", nullptr));
        menuCybersec_Toolkit->setTitle(QCoreApplication::translate("MainWindow", "Cybersec Toolkit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
