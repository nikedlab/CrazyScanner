#include <QApplication>
#include "main.h"
#include "server.h"
#include "filelogger.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server *server = new Server;
    server->startServer();
    FileLogger logger;
    logger.writeLog("main");
    return a.exec();
}
