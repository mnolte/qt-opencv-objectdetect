#include "mainwindow.h"
#include <QApplication>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("ComputerVision");
    app.setOrganizationName("Marc Nolte");
    app.setOrganizationDomain("marcnolte.nl");

    MainWindow w;
    w.show();

    return app.exec();
}
