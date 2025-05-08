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
    QStackedWidget *stackedWidget_2;
    QWidget *page;
    QPushButton *hashButton;
    QPushButton *compareButton;
    QPushButton *fileButton;
    QPushButton *uploadButton;
    QPushButton *switchToDirButton;
    QWidget *page_3;
    QPushButton *switchToSingleButton;
    QPushButton *dir_fileButton;
    QPushButton *dir_compareButton;
    QPushButton *dir_hashButton;
    QPushButton *dir_uploadButton;
    QMenuBar *menubar;
    QMenu *menuCybersec_Toolkit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("/* Base application style */\n"
"QWidget {\n"
"    background-color: #121212;\n"
"    color: #e0e0e0;\n"
"    font-family: comic-sans;\n"
"    font-size: 8pt;\n"
"}\n"
"\n"
"/* Buttons */\n"
"QPushButton {\n"
"    background-color: #1f1f1f;\n"
"    color: #ffffff;\n"
"    border: 1px solid #3c3c3c;\n"
"    border-radius: 6px;\n"
"    padding: 4px 8px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #2a2a2a;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #3a3a3a;\n"
"}\n"
"\n"
"/* Line Edits and Text Areas */\n"
"QLineEdit, QTextEdit, QPlainTextEdit {\n"
"    background-color: #1f1f1f;\n"
"    color: #ffffff;\n"
"    border: 1px solid #3c3c3c;\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"}\n"
"\n"
"/* Labels */\n"
"QLabel {\n"
"    color: #e0e0e0;\n"
"}\n"
"\n"
"/* Combo Boxes */\n"
"QComboBox {\n"
"    background-color: #1f1f1f;\n"
"    color: #ffffff;\n"
"    border: 1px solid #3c3c3c;\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"}\n"
"\n"
"/* Message Boxes */\n"
""
                        "QMessageBox {\n"
"    background-color: #1f1f1f;\n"
"    color: #ffffff;\n"
"}\n"
"\n"
"/* Group Boxes */\n"
"QGroupBox {\n"
"    border: 1px solid #3c3c3c;\n"
"    border-radius: 6px;\n"
"    margin-top: 10px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top center;\n"
"    padding: 0 3px;\n"
"}\n"
"\n"
"/* Scrollbars */\n"
"QScrollBar:vertical, QScrollBar:horizontal {\n"
"    background: #1f1f1f;\n"
"    border: none;\n"
"    width: 12px;\n"
"    height: 12px;\n"
"    margin: 0px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical, QScrollBar::handle:horizontal {\n"
"    background: #3c3c3c;\n"
"    min-height: 20px;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"QScrollBar::add-line, QScrollBar::sub-line {\n"
"    background: none;\n"
"    border: none;\n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        fileScanButton = new QPushButton(centralwidget);
        fileScanButton->setObjectName("fileScanButton");
        fileScanButton->setGeometry(QRect(10, 40, 181, 41));
        fileScanButton->setMaximumSize(QSize(181, 51));
        stegButton = new QPushButton(centralwidget);
        stegButton->setObjectName("stegButton");
        stegButton->setGeometry(QRect(10, 100, 181, 41));
        stegButton->setStyleSheet(QString::fromUtf8(""));
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
        stackedWidget_2 = new QStackedWidget(page_2);
        stackedWidget_2->setObjectName("stackedWidget_2");
        stackedWidget_2->setGeometry(QRect(140, 180, 401, 331));
        page = new QWidget();
        page->setObjectName("page");
        hashButton = new QPushButton(page);
        hashButton->setObjectName("hashButton");
        hashButton->setGeometry(QRect(50, 110, 91, 41));
        compareButton = new QPushButton(page);
        compareButton->setObjectName("compareButton");
        compareButton->setGeometry(QRect(180, 110, 91, 41));
        fileButton = new QPushButton(page);
        fileButton->setObjectName("fileButton");
        fileButton->setGeometry(QRect(110, 50, 101, 31));
        uploadButton = new QPushButton(page);
        uploadButton->setObjectName("uploadButton");
        uploadButton->setGeometry(QRect(290, 300, 101, 24));
        switchToDirButton = new QPushButton(page);
        switchToDirButton->setObjectName("switchToDirButton");
        switchToDirButton->setGeometry(QRect(10, 10, 301, 31));
        stackedWidget_2->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        switchToSingleButton = new QPushButton(page_3);
        switchToSingleButton->setObjectName("switchToSingleButton");
        switchToSingleButton->setGeometry(QRect(10, 10, 301, 31));
        dir_fileButton = new QPushButton(page_3);
        dir_fileButton->setObjectName("dir_fileButton");
        dir_fileButton->setGeometry(QRect(110, 50, 101, 31));
        dir_compareButton = new QPushButton(page_3);
        dir_compareButton->setObjectName("dir_compareButton");
        dir_compareButton->setGeometry(QRect(180, 110, 91, 41));
        dir_hashButton = new QPushButton(page_3);
        dir_hashButton->setObjectName("dir_hashButton");
        dir_hashButton->setGeometry(QRect(50, 110, 91, 41));
        dir_uploadButton = new QPushButton(page_3);
        dir_uploadButton->setObjectName("dir_uploadButton");
        dir_uploadButton->setGeometry(QRect(290, 300, 101, 24));
        stackedWidget_2->addWidget(page_3);
        stackedWidget->addWidget(page_2);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 18));
        menuCybersec_Toolkit = new QMenu(menubar);
        menuCybersec_Toolkit->setObjectName("menuCybersec_Toolkit");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuCybersec_Toolkit->menuAction());

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);
        stackedWidget_2->setCurrentIndex(1);


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
        hashButton->setText(QCoreApplication::translate("MainWindow", "Create Hash", nullptr));
        compareButton->setText(QCoreApplication::translate("MainWindow", "Compare File", nullptr));
        fileButton->setText(QCoreApplication::translate("MainWindow", "Select File", nullptr));
        uploadButton->setText(QCoreApplication::translate("MainWindow", "Upload Hash", nullptr));
        switchToDirButton->setText(QCoreApplication::translate("MainWindow", "Single File Scan", nullptr));
        switchToSingleButton->setText(QCoreApplication::translate("MainWindow", "Directory File Scan", nullptr));
        dir_fileButton->setText(QCoreApplication::translate("MainWindow", "Select Directory", nullptr));
        dir_compareButton->setText(QCoreApplication::translate("MainWindow", "Compare Files", nullptr));
        dir_hashButton->setText(QCoreApplication::translate("MainWindow", "Create Hashes", nullptr));
        dir_uploadButton->setText(QCoreApplication::translate("MainWindow", "Upload Hashes", nullptr));
        menuCybersec_Toolkit->setTitle(QCoreApplication::translate("MainWindow", "CySec Toolkit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
