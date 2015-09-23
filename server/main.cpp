#include <QApplication>
#include "main.h"
#include "server.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server *server = new Server;
    server->startServer();
    return a.exec();
}
