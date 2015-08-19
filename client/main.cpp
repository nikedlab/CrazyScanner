#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow *mainActivity = new MainWindow();
    mainActivity->show();
    mainActivity->setGeometry(mainActivity->geometry());

    return app.exec();
}
