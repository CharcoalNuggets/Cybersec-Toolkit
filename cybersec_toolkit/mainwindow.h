#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_fileScanButton_clicked();
    void on_stegButton_clicked();

//file scanner functions
    void on_fileButton_clicked();
    void on_hashButton_clicked();
    void on_compareButton_clicked();

//steganography functions
    void on_imageButton_clicked();
    void on_encodeButton_clicked();
    void on_decodeButton_clicked();



private:
    Ui::MainWindow *ui;
    QImage image;
    QString selectedFilePath;
};
#endif // MAINWINDOW_H
