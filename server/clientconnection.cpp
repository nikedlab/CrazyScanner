#include "clientconnection.h"

#include <QDebug>
#include <QFileInfo>
#include <QDirIterator>
#include <QDir>
#include <QThreadPool>


ClientConnection::ClientConnection(int socketDescriptor, QObject *parent) : QObject(parent), socketDescriptor(socketDescriptor)
{
    this->sigMap = prepareMap();
}

void ClientConnection::run() {
    client = new QTcpSocket(this);

    qDebug() << "New socket created!";

    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));

    client->setSocketDescriptor(socketDescriptor);
}

void ClientConnection::readyRead() {

    QString path = QString::fromUtf8(client->readLine()).trimmed();
    qDebug() << "Path: " + path;

    client->write(QString("Path: " + path + "\n").toUtf8());

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

void ClientConnection::printVerdict(QByteArray verdict) {
    qDebug() << verdict;
    client->write(QString(verdict + "\n").toUtf8());
}

QMap<QString, QString> *ClientConnection::prepareMap() {
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

void ClientConnection::disconnected() {
    qDebug() << "Client disconnected";
}
