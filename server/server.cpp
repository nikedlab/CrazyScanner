#include "server.h"

#include "clientconnection.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{

}

void Server::startServer(){
    int port = 1024;
    if (!this->listen(QHostAddress::Any, port)) {
        qDebug() << "Server start error!";
    } else {
        qDebug() << "Server start OK. Port: " << port;
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QThread *thread = new QThread();
    ClientConnection *connection = new ClientConnection(socketDescriptor, this);
    connection->moveToThread(thread);
    connect(thread, SIGNAL(started()), connection, SLOT(run()));
    thread->start();
    qDebug() << "New connection";
}

Server::~Server(){

}

