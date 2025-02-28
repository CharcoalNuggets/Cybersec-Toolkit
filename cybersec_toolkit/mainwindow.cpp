#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStackedWidget>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fileScanButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_stegButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_fileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select a file to scan");
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "File Selected", "Scanning(wip): " + fileName);
    }
}

void MainWindow::on_scanButton_clicked()
{

}

