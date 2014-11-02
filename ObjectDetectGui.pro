#-------------------------------------------------
#
# Project created by QtCreator 2014-10-30T15:02:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QT5
TEMPLATE = app

SOURCES += main.cpp\
mainwindow.cpp \
aboutdialog.cpp

HEADERS  += mainwindow.h \
aboutdialog.h

FORMS    += mainwindow.ui \
aboutdialog.ui

INCLUDEPATH += /usr/local/include/opencv2

LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
-lopencv_ml \
-lopencv_video \
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_objdetect \
-lopencv_contrib \
-lopencv_legacy \
-lopencv_flann \
-lopencv_nonfree
