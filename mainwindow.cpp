#include "mainwindow.h"
#include "aboutdialog.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QDateTime>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set form values
    hMin = 0, sMin = 0, vMin = 170, hMax = 90, sMax = 127, vMax = 256;
    ui->spinBoxHMin->setValue(hMin);
    ui->spinBoxHMax->setValue(hMax);
    ui->spinBoxSMin->setValue(sMin);
    ui->spinBoxSMax->setValue(sMax);
    ui->spinBoxVMin->setValue(vMin);
    ui->spinBoxVMax->setValue(vMax);

    // Start capture
    capWebcam.open(0);
    if(capWebcam.isOpened() == false)
    {
        ui->textConsole->appendPlainText("Error: Webcam could not be accessed");
        return;
    }
    else
    {
        ui->textConsole->appendPlainText("Webcam started...");
    }

    // Start timer
    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(ballTracking()));
    tmrTimer->start(20);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::ballTracking()
{
    capWebcam.read(matOriginal);
    if(matOriginal.empty() == true)
    {
        ui->textConsole->appendPlainText("Error: Webcam frame could not be accessed"); return;
    }

    cv::cvtColor(matOriginal, matProcessed, CV_BGR2HSV);
    cv::inRange(matOriginal, cv::Scalar(hMin,sMin,vMin), cv::Scalar(hMax,sMax,vMax), matProcessed);
    cv::GaussianBlur(matProcessed, matProcessed, cv::Size(9,9), 5, 5);
    cv::threshold(matProcessed, matProcessed, 120, 255, CV_THRESH_BINARY);
    cv::HoughCircles(matProcessed, vecCircles, CV_HOUGH_GRADIENT, 2, matProcessed.rows / 4, 100, 50, 10, 400);

    for(itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++)
    {
        circle(matOriginal, cv::Point((*itrCircles)[0], (*itrCircles)[1]), (*itrCircles)[2], cv::Scalar(0,256,0), 2, 8, 0);
        ui->textConsole->appendPlainText(QString("Position: (") +
                                         QString::number((*itrCircles)[0]).rightJustified(4, ' ') +
                                         QString("x") +
                                         QString::number((*itrCircles)[1]).rightJustified(4, ' ') +
                                         QString(") Radius: ") +
                                         QString::number((*itrCircles)[2], 'f', 3).rightJustified(7, ' '));
    }

    cv::cvtColor(matOriginal, matOriginal, CV_BGR2RGB);
    QImage qimgOriginal((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    QImage qimgProcessed((uchar*)matProcessed.data, matProcessed.cols, matProcessed.rows, matProcessed.step, QImage::Format_Indexed8);

    ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));
    ui->lblProcessed->setPixmap(QPixmap::fromImage(qimgProcessed));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_btnPauseResume_clicked()
{
    if(tmrTimer->isActive() == true)
    {
        tmrTimer->stop();
        ui->btnPauseResume->setText("Resume");
        ui->textConsole->appendPlainText("Webcam paused...");
    }
    else
    {
        tmrTimer->start(20);
        ui->btnPauseResume->setText("Pause");
        ui->textConsole->appendPlainText("Webcam resumed...");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_btnSnapshot_clicked()
{
    // Read frame
    capWebcam.read(matOriginal);
    if(matOriginal.empty() == true) return;

    // Set path
    QString storePath = "/home/marc/Afbeeldingen/OpenCV/Snapshots/";
    QString fileName  = "snapshot_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".jpg";

    // Create path directories (if not exists)
    if(QDir(storePath).exists() == false)
    {
        QDir().mkpath(storePath);
    }

    // Write snapshot
    if(cv::imwrite((storePath + fileName).toUtf8().constData(), matOriginal) == false)
    {
        ui->textConsole->appendPlainText("Error: Snapshot failed. Could not write to " + storePath); return;
    }

    ui->textConsole->appendPlainText("Snapshot stored: " + storePath + fileName);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionAbout_triggered()
{
    AboutDialog aboutDialog;
    aboutDialog.setModal(true);
    aboutDialog.exec();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_spinBoxHMin_valueChanged(int spinBoxValue)
{
    hMin = spinBoxValue;
    if(hMin > hMax)
    {
        hMax = hMin;
        ui->spinBoxHMax->setValue(hMax);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_spinBoxHMax_valueChanged(int spinBoxValue)
{
    hMax = spinBoxValue;
    if(hMax < hMin)
    {
        hMin = hMax;
        ui->spinBoxHMin->setValue(hMin);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_spinBoxSMin_valueChanged(int spinBoxValue)
{
    sMin = spinBoxValue;
    if(sMin > sMax)
    {
        sMax = sMin;
        ui->spinBoxSMax->setValue(sMax);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_spinBoxSMax_valueChanged(int spinBoxValue)
{
    sMax = spinBoxValue;
    if(sMax < sMin)
    {
        sMin = sMax;
        ui->spinBoxSMin->setValue(sMin);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_spinBoxVMin_valueChanged(int spinBoxValue)
{
    vMin = spinBoxValue;
    if(vMin > vMax)
    {
        vMax = vMin;
        ui->spinBoxVMax->setValue(vMax);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_spinBoxVMax_valueChanged(int spinBoxValue)
{
    vMax = spinBoxValue;
    if(vMax < vMin)
    {
        vMin = vMax;
        ui->spinBoxVMin->setValue(vMin);
    }
}
