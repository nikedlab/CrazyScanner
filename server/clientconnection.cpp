#include "clientconnection.h"

#include <QDebug>
#include <QFileInfo>
#include <QDirIterator>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>



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

    writeToClient(INIT, QString::number(getFilesCount(path)));

    QFileInfo fileInfo(path);
    QThreadPool *pool = QThreadPool::globalInstance();
    if (fileInfo.exists()) {
        if (fileInfo.isFile()) {
            doScan(fileInfo.absoluteFilePath(), pool);
        } else {
            QDirIterator dirIterator(path, QDir::NoDotAndDotDot | QDir::AllEntries, QDirIterator::Subdirectories);
            while (dirIterator.hasNext()) {
                QString file = dirIterator.next();
                QFileInfo fileInfo(file);
                if (fileInfo.isDir()) {
                    continue;
                }
                doScan(file, pool);
            }
        }
    }
    pool->waitForDone();
    writeToClient(DONE, QString("done"));
    client->close();
}

int ClientConnection::getFilesCount(QString dirPath) {
    int result = 0;
    QDirIterator dirIterator(dirPath, QDir::NoDotAndDotDot | QDir::AllEntries, QDirIterator::Subdirectories);
    while (dirIterator.hasNext()) {
        QString file = dirIterator.next();
        QFileInfo fileInfo(file);
        if (fileInfo.isDir()) {
            continue;
        }
        result++;
    }
    return result;
}

void ClientConnection::writeToClient(ResponceTypes type, QString message) {
    QJsonObject root;
    root["message"] = message;

    switch (type) {
    case INIT:
        root["type"] = "init";
        break;
    case DONE:
        root["type"] = "done";
        break;
    }
    client->write(QString(QJsonDocument(root).toJson(QJsonDocument::Compact) + "\n").toUtf8());
}

void ClientConnection::doScan(QString filePath, QThreadPool *pool) {
    FileProcessor *processor = new FileProcessor(filePath, sigMap);
    processor->setAutoDelete(true);
    connect(processor, SIGNAL(sendVerdict(QByteArray)), this, SLOT(printVerdict(QByteArray)), Qt::DirectConnection);
    pool->start(processor);
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
