#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Ui
{
    class MainWindow;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;

        cv::VideoCapture capWebcam;

        int hMin, sMin, vMin, hMax, sMax, vMax;

        cv::Mat matOriginal;
        cv::Mat matProcessed;

        QImage qimgOriginal;
        QImage qimgProcessed;

        std::vector<cv::Vec3f> vecCircles;
        std::vector<cv::Vec3f>::iterator itrCircles;

        QTimer* tmrTimer;

    public slots:
        void ballTracking();

    private slots:
        void on_btnPauseResume_clicked();
        void on_btnSnapshot_clicked();
        void on_actionExit_triggered();
        void on_actionAbout_triggered();
        void on_spinBoxHMin_valueChanged(int arg1);
        void on_spinBoxHMax_valueChanged(int arg1);
        void on_spinBoxSMin_valueChanged(int arg1);
        void on_spinBoxSMax_valueChanged(int arg1);
        void on_spinBoxVMin_valueChanged(int arg1);
        void on_spinBoxVMax_valueChanged(int arg1);
};

#endif // MAINWINDOW_H
