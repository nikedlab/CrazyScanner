#include "server.h"
#include "fileprocessor.h"
#include <QTcpSocket>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QDirIterator>
#include <QDir>

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(5);
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
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketDescriptor);

    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << "New connection";
}

void Server::readyRead() {
    client = (QTcpSocket*)sender();
    QString path = QString::fromUtf8(client->readLine()).trimmed();
    qDebug() << "Path: " + path;
    FileProcessor *processor = new FileProcessor(this);
    connect(processor, SIGNAL(sendVerdict(QByteArray)), this, SLOT(printVerdict(QByteArray)));

    processor->setAutoDelete(true);

    QFileInfo fileInfo(path);
    if (fileInfo.exists()) {
        if (fileInfo.isFile()) {
            //processor.processFile(fileInfo.absoluteFilePath());

            processor->filePath = fileInfo.absoluteFilePath();
            pool->start(processor);
        } else {
            QDirIterator dirIterator(path, QDir::NoDotAndDotDot | QDir::AllEntries, QDirIterator::Subdirectories);
            while (dirIterator.hasNext()) {
                processor->filePath = dirIterator.next();
                pool->start(processor);
                //processor.processFile(dirIterator.next());
            }
        }
    }

    delete processor;
    client->close();
}

void Server::printVerdict(QByteArray verdict) {
    qDebug() << verdict;
    client->write(QString(verdict + "\n").toUtf8());
}

void Server::disconnected() {
    qDebug() << "Client disconnected";
}

Server::~Server(){

}

