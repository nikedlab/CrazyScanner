#include "server.h"

#include "clientthread.h"
#include <QTcpSocket>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// http://stackoverflow.com/questions/4874844/writing-data-to-a-tcpsocket-in-own-thread
// https://github.com/e1sm/QTAsyncServer
// https://github.com/Manoj-red-hat/LANTORRENT/tree/49fc01ac9c59c083fb635a06f025768fc1f6320a/DownloadWindow


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
    //QTcpSocket *client = new QTcpSocket();
    //client->setSocketDescriptor(socketDescriptor);

    QThread *t = new QThread();

    ClientThread *clientThread = new ClientThread(socketDescriptor, this);

    clientThread->moveToThread(t);

//    connect(clientThread, SIGNAL(readyRead()), this, SLOT(readyRead()));
//    connect(clientThread, SIGNAL(disconnected()), this, SLOT(disconnected()));

    t->start();
    qDebug() << "New connection";
}



Server::~Server(){

}

