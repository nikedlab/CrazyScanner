#include "clientthread.h"

#include <QDebug>
#include <QFileInfo>
#include <QDirIterator>
#include <QDir>
#include <QThreadPool>


ClientThread::ClientThread(int socketDescriptor,QObject *parent) :
    QThread(parent),socketDescriptor(socketDescriptor)
{
    this->sigMap = prepareMap();
    this->socketDescriptor = socketDescriptor;
}

void ClientThread::run() {
    socket = new QTcpSocket(this);

    qDebug() << "New socket created!";


    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(socketDescriptor);
}

void ClientThread::readyRead() {
    QTcpSocket *client = (QTcpSocket*) sender();

    QString path = QString::fromUtf8(client->readLine()).trimmed();
    qDebug() << "Path: " + path;


    QFileInfo fileInfo(path);
    QThreadPool *pool = QThreadPool::globalInstance();
    if (fileInfo.exists()) {
        if (fileInfo.isFile()) {
            //doScan(fileInfo.absoluteFilePath());
        } else {
            QDirIterator dirIterator(path, QDir::NoDotAndDotDot | QDir::AllEntries, QDirIterator::Subdirectories);
            while (dirIterator.hasNext()) {
                FileProcessor *processor = new FileProcessor(dirIterator.next(), sigMap);
                processor->setAutoDelete(true);
                connect(processor, SIGNAL(sendVerdict(QByteArray)), this, SLOT(printVerdict(QByteArray)), Qt::DirectConnection);
                pool->start(processor);
            }
        }
    }
    pool->waitForDone();
    client->close();
}

QMap<QString, QString> *ClientThread::prepareMap() {
    QMap<QString, QString> *sigMap = new QMap<QString, QString>();
    QFile baseFile(":/av/base/base.txt");
    if (baseFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&baseFile);
        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(".");
            QString uuid = parts.takeAt(1);
            sigMap->insert(parts.takeAt(0), uuid);
        }
        baseFile.close();
    } else {
        qDebug() << "Error open base.txt";
    }
    return sigMap;
}

void ClientThread::printVerdict(QByteArray verdict) {
    qDebug() << verdict;
    QTcpSocket *client = (QTcpSocket*) sender();

    client->write(QString(verdict + "\n").toUtf8());
}

void ClientThread::disconnected() {
    qDebug() << "Client disconnected";
}
