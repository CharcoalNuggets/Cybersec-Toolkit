#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStackedWidget>   //
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QDebug>
#include <QCryptographicHash>

#include <QNetworkAccessManager>    //firebase networking
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <QDir> //directories
#include <QDirIterator>
#include <QTimer>

#include <QProgressDialog>  //progress bar

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("cybersec_toolkit.exe");
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


void MainWindow::on_switchToDirButton_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}


void MainWindow::on_switchToSingleButton_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}


//********* single file scanner functions *****************************

void MainWindow::on_fileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select a file to scan");
    if(!fileName.isEmpty()) {
        selectedFilePath = fileName;
        QMessageBox::information(this, "File selected", "Operating on: " + selectedFilePath);
    }
}

/*//break in case of emergency
void MainWindow::on_uploadButton_clicked()
{
    if(lastComputedHash.isEmpty()){
        QMessageBox::warning(this, "Error", "Please hash a file before uploading.");
        return;
    }

    QUrl url("https://firestore.googleapis.com/v1/projects/cs490-f7072/databases/(default)/documents/hashes");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject doc;
    QJsonObject fields;
    QJsonObject hashField;
    hashField["stringValue"] = lastComputedHash;
    fields["hash"] = hashField;
    doc["fields"] = fields;

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, QJsonDocument(doc).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if(reply->error() == QNetworkReply::NoError){
            QMessageBox::information(this, "Success", "Hash uploaded to Firestore.");
        }
        else{
            QMessageBox::warning(this, "Upload failed", reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}
*/

void MainWindow::on_uploadButton_clicked()
{
    if(lastComputedHash.isEmpty()){
        QMessageBox::warning(this, "Error", "Please hash a file before uploading.");
        return;
    }

    QUrl url("https://firestore.googleapis.com/v1/projects/cs490-f7072/databases/(default)/documents/hashes");
    QNetworkRequest request(url);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if(reply->error() != QNetworkReply::NoError){
            QMessageBox::warning(this, "Error", "Failed to check existing hashes: " + reply->errorString());
            reply->deleteLater();
            manager->deleteLater();
            return;
        }

        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonArray documents = jsonDoc.object().value("documents").toArray();

        QSet<QString> existingHashes;
        for(const QJsonValue &docVal : documents){//fill set with firestore contents
            QJsonObject fields = docVal.toObject().value("fields").toObject();
            QString hash = fields.value("hash").toObject().value("stringValue").toString();
            existingHashes.insert(hash);
        }

        if(existingHashes.contains(lastComputedHash)){
            QMessageBox::information(this, "Info", "Hash already exists in Firestore.");
        }
        else{//upload only if it's not a duplicate
            QJsonObject doc;
            QJsonObject fields;
            QJsonObject hashField;
            hashField["stringValue"] = lastComputedHash;
            fields["hash"] = hashField;
            doc["fields"] = fields;

            QNetworkRequest postRequest(url);
            postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QNetworkReply *postReply = manager->post(postRequest, QJsonDocument(doc).toJson());

            connect(postReply, &QNetworkReply::finished, this, [=]() {
                if(postReply->error() == QNetworkReply::NoError){
                    QMessageBox::information(this, "Success", "Hash uploaded to Firestore.");
                }
                else{
                    QMessageBox::warning(this, "Upload failed", postReply->errorString());
                }
                postReply->deleteLater();
                manager->deleteLater();
            });
        }
        reply->deleteLater();
    });
}


void MainWindow::on_hashButton_clicked()
{
    if(selectedFilePath.isEmpty()){
        QMessageBox::warning(this, "No file", "Please select a file first.");
        return;
    }

    QFile file(selectedFilePath);
    if(!file.open(QFile::ReadOnly)){
        QMessageBox::warning(this, "Error", "Unable to open file for hashing.");
        return;
    }

    QCryptographicHash hash(QCryptographicHash::Sha1);
    if(!hash.addData(&file)){
        QMessageBox::warning(this, "Error", "Failed to hash file.");
        file.close();
        return;
    }
    file.close();

    QString hexHash = hash.result().toHex();
    QMessageBox::information(this, "File Hash", "SHA-1:\n" + hexHash);
    lastComputedHash = hexHash; //store for upload
}

void MainWindow::on_compareButton_clicked()
{
    if(lastComputedHash.isEmpty()) {
        QMessageBox::warning(this, "No hash", "Please hash a file before comparing.");
        return;
    }

    QUrl url("https://firestore.googleapis.com/v1/projects/cs490-f7072/databases/(default)/documents/hashes?pageSize=1000");
    QNetworkRequest request(url);
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(this, "Compare failed", reply->errorString());
            reply->deleteLater();
            manager->deleteLater();
            return;
        }

        QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject responseObj = responseDoc.object();
        QJsonArray documents = responseObj["documents"].toArray();

        bool foundMatch = false;
        for(const QJsonValue& docVal : documents){
            QJsonObject doc = docVal.toObject();
            QJsonObject fields = doc["fields"].toObject();
            QString hashValue = fields["hash"].toObject()["stringValue"].toString();
            if (hashValue == lastComputedHash) {
                foundMatch = true;
                break;
            }
        }

        if(foundMatch){
            QMessageBox::information(this, "Compare Result", "Match found in Firestore.");
        }
        else{
            QMessageBox::information(this, "Compare Result", "No match found.");
        }

        reply->deleteLater();
        manager->deleteLater();
    });
}


//********* directory file scanner functions *****************************

void MainWindow::on_dir_fileButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "Select Directory to Scan");

    if(!dirPath.isEmpty()){
        selectedDirPath = dirPath;
        QMessageBox::information(this, "Directory selected", "Operating on: " + selectedDirPath);
    }
}

void MainWindow::on_dir_hashButton_clicked()
{
    if(selectedDirPath.isEmpty()){
        QMessageBox::warning(this, "No directory", "Please select a directory first.");
        return;
    }

    dirFileHashes.clear(); //clear prior scan
    QDirIterator counterIt(selectedDirPath, QDir::Files, QDirIterator::Subdirectories);
    QStringList allFiles;   //track number of files for progress updater
    while(counterIt.hasNext()){
        allFiles << counterIt.next();
    }

    int totalFiles = allFiles.size();
    if(totalFiles == 0){    //error check no files
        QMessageBox::information(this, "Empty", "No files found in the directory.");
        return;
    }

    QProgressDialog progress("Hashing files...", "Cancel", 0, totalFiles, this);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setMinimumDuration(0);
    progress.setWindowTitle("Please wait");
    progress.setAutoClose(true);
    progress.setCancelButton(nullptr);  //hides cancel button

    int count = 0;
    for(const QString &filePath : allFiles){
        QFile file(filePath);
        if(file.open(QFile::ReadOnly)){
            QCryptographicHash hash(QCryptographicHash::Sha1);

            if(hash.addData(&file)){
                QString hexHash = hash.result().toHex();
                dirFileHashes.append(qMakePair(filePath, hexHash));
                count++;
            }
            file.close();
        }

        progress.setValue(count);
        qApp->processEvents();  //allows the dialog to repaint
    }
    progress.setValue(totalFiles);  //set progress bar to 100% on completion

    QMessageBox::information(this, "Hashing Complete", QString("Hashed %1 file(s).").arg(count));
}


void MainWindow::on_dir_uploadButton_clicked()
{
    if(dirFileHashes.isEmpty()){
        QMessageBox::warning(this, "No Hashes", "Please hash directory files before uploading.");
        return;
    }

    QUrl url("https://firestore.googleapis.com/v1/projects/cs490-f7072/databases/(default)/documents/hashes");
    QNetworkRequest request(url);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=](){
        if(reply->error() != QNetworkReply::NoError){
            QMessageBox::warning(this, "Error", "Failed to fetch existing hashes: " + reply->errorString());
            reply->deleteLater();
            manager->deleteLater();
            return;
        }

        QSet<QString> existingHashes;
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonArray documents = jsonDoc.object().value("documents").toArray();

        for(const QJsonValue &docVal : documents){  //copy firestore contents
            QJsonObject fields = docVal.toObject().value("fields").toObject();
            QString hash = fields.value("hash").toObject().value("stringValue").toString();
            existingHashes.insert(hash);
        }

        int uploadedCount = 0;
        for(const auto &pair : dirFileHashes){
            const QString &hash = pair.second;
            if(!existingHashes.contains(hash)){
                QJsonObject doc;
                QJsonObject fields;
                QJsonObject hashField;
                hashField["stringValue"] = hash;
                fields["hash"] = hashField;
                doc["fields"] = fields;

                QNetworkRequest postRequest(url);
                postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QNetworkReply *postReply = manager->post(postRequest, QJsonDocument(doc).toJson());
                connect(postReply, &QNetworkReply::finished, this, [=]() {
                    postReply->deleteLater();
                });

                uploadedCount++;
            }
        }

        QMessageBox::information(this, "Upload complete",
                                 uploadedCount == 0 ? "All hashes already exist." : QString("Uploaded %1 new hash%2.")
                                                                                        .arg(uploadedCount)
                                                                                        .arg(uploadedCount == 1 ? "" : "es"));  //handle unknown plurality

        reply->deleteLater();
        manager->deleteLater();
    });
}

void MainWindow::on_dir_compareButton_clicked()
{
    if(dirFileHashes.isEmpty()){
        QMessageBox::warning(this, "Error", "Please hash a directory first.");
        return;
    }

    QUrl url("https://firestore.googleapis.com/v1/projects/cs490-f7072/databases/(default)/documents/hashes");
    QNetworkRequest request(url);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=](){

        if(reply->error() != QNetworkReply::NoError){
            QMessageBox::warning(this, "Error", "Failed to retrieve hashes: " + reply->errorString());
            reply->deleteLater();
            manager->deleteLater();
            return;
        }

        QSet<QString> firestoreHashes;
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonArray documents = jsonDoc.object().value("documents").toArray();

        for(const QJsonValue &docVal : documents){  //copy firestore contents
            QJsonObject fields = docVal.toObject().value("fields").toObject();
            QString hash = fields.value("hash").toObject().value("stringValue").toString();
            firestoreHashes.insert(hash);
        }

        QStringList matchedFilePaths;
        for(const auto &pair : dirFileHashes){  //hold onto file paths for removal
            const QString &filePath = pair.first;
            const QString &localHash = pair.second;

            if(firestoreHashes.contains(localHash)){
                matchedFilePaths << filePath;
            }
        }

        if(matchedFilePaths.isEmpty()){
            QMessageBox::information(this, "No Matches", "No matching hashes found in Firestore.");
        }
        else{
            QString msg = QString("Matched %1 file(s):\n\nDo you want to remove them?")
                              .arg(matchedFilePaths.size());
            //.arg(matchedFilePaths.join("\n"));    //not sure about listing out all filepaths. looks ugly and possibly pointless

            QMessageBox::StandardButton reply = QMessageBox::question(this, "Matches Found", msg,
                                                                      QMessageBox::Yes | QMessageBox::No);

            if(reply == QMessageBox::Yes){
                int deletedCount = 0;
                for(const QString &filePath : matchedFilePaths){
                    if(QFile::remove(filePath)){
                        deletedCount++;
                    }
                }

                QMessageBox::information(this, "Removal Complete",
                                         QString("Deleted %1 out of %2 matched files.")
                                             .arg(deletedCount)
                                             .arg(matchedFilePaths.size()));    //pleasantly similar to printf formatting. am i retarded
            }
        }

        reply->deleteLater();
        manager->deleteLater(); //yes
    });
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





