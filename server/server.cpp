#include "server.h"

#include "clientconnection.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    logger = new FileLogger();
}

void Server::startServer(){
    logger->writeLog("Attempt to start server");

    int port = 1024;
    if (!this->listen(QHostAddress::Any, port)) {
        logger->writeLog("Server start error!");
        qDebug() << "Server start error!";
    } else {
        logger->writeLog("Server start OK. Port: " + QString::number(port));
        qDebug() << "Server start OK. Port: " << port;
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    logger->writeLog("New connection");
    thread = new QThread();
    ClientConnection *connection = new ClientConnection(socketDescriptor, this);
    connection->moveToThread(thread);
    connect(thread, SIGNAL(started()), connection, SLOT(run()));
    thread->start();
    qDebug() << "New connection";
}

Server::~Server(){
    delete logger;
    thread->exit();
    delete thread;
}

