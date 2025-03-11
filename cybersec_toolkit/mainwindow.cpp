#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStackedWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QDebug>

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
    ui->stackedWidget->setCurrentIndex(0);//stack is in fact zero indexed
}


void MainWindow::on_fileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select a file to scan");
    if(!fileName.isEmpty()) {
        QMessageBox::information(this, "File selected", "Scanning(wip): " + fileName);
    }
}

void MainWindow::on_scanButton_clicked()
{
//to be implemented during/after break
}

//********** steganography functions ****************************

void MainWindow::on_imageButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open image", "", "Image (*.png *jpg *jpeg)");
    if(filePath.isEmpty()) return;

    image.load(filePath);
    if(image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    ui->imageLabel->setPixmap(QPixmap::fromImage(image).scaled(256, 256, Qt::KeepAspectRatio)); //scale any image to 256x256 pixel size
    QMessageBox::information(this, "Success", "Image loaded successfully.");
}


void MainWindow::on_encodeButton_clicked() {    //can use built in QImage library for image manipulation
    if(image.isNull()) {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }

    QString message = ui->messageTextEdit->toPlainText();
    QByteArray data = message.toUtf8();//QByteArray requires UTF8 conversion
    data.append('\0');  //null terminator to mark end

    int width = image.width();
    int height = image.height();
    int index = 0;
    int bitPosition = 0;  //track bit position within the character

    for(int y = 0; y < height && index < data.size(); y++) {
        for(int x = 0; x < width && index < data.size(); x++) {

            QRgb pixel = image.pixel(x, y); //extract rgb value
            int red = qRed(pixel);  //store red value
            int green = qGreen(pixel);  //store green value
            int blue = qBlue(pixel);    //store blue value

            char character = data[index];

            if(bitPosition == 0) {  //extract the character and spread its bits across 3 pixels
                red = (red & ~1) | ((character >> 7) & 1);
                green = (green & ~1) | ((character >> 6) & 1);  //bitwise is gross
                blue = (blue & ~1) | ((character >> 5) & 1);
            }
            else if(bitPosition == 1) {
                red = (red & ~1) | ((character >> 4) & 1);  //shave off the last bit of the color value; shift nth bit of the message char to the right; bitwise addition to combine
                green = (green & ~1) | ((character >> 3) & 1);
                blue = (blue & ~1) | ((character >> 2) & 1);
            }
            else if(bitPosition == 2) {
                red = (red & ~1) | ((character >> 1) & 1);
                green = (green & ~1) | ((character >> 0) & 1);
            }

            image.setPixel(x, y, qRgb(red, green, blue));

            bitPosition++;
            if(bitPosition == 3) {  //reset
                bitPosition = 0;
                index++;    //move to next character of message
            }
        }
    }

    QString outputPath = QFileDialog::getSaveFileName(this, "Save image", "", "Image (*.png *jpg *jpeg)");

    if(!outputPath.isEmpty()) {
        image.save(outputPath);
        QMessageBox::information(this, "Success", "Message encoded and image saved.");
    }
}


void MainWindow::on_decodeButton_clicked() {
    if(image.isNull()) {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }

    QByteArray message;
    int width = image.width();
    int height = image.height();
    char character = 0;
    int bitPosition = 7; //reverse order


    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            QRgb pixel = image.pixel(x, y);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);


            character |= ((red & 1) << bitPosition--);  //decode in the same order as encoded or spaghetti happens
            character |= ((green & 1) << bitPosition--);    //bitwise add the 8th bit that has now been shifted to the original bitPosition
            character |= ((blue & 1) << bitPosition--);

            if(bitPosition < 0) {
                if (character == '\0') {
                    ui->decodeLabel->setText("Decoded message: " + QString::fromUtf8(message));
                    return;
                }
                message.append(character);
                character = 0;
                bitPosition = 7;
            }
        }
    }
    ui->decodeLabel->setText("Decoded message: " + QString::fromUtf8(message));
}
