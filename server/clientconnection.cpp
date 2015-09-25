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
    logger = new FileLogger();
    client = new QTcpSocket(this);
}

void ClientConnection::run() {


    qDebug() << "New socket created!";
    logger->writeLog("New socket created!");

    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));

    client->setSocketDescriptor(socketDescriptor);
}

void ClientConnection::readyRead() {

    QString path = QString::fromUtf8(client->readLine()).trimmed();
    qDebug() << "Path: " + path;
    logger->writeLog("Path: " + path);
    int filesCount = getFilesCount(path);
    writeToClient(INIT, QString::number(filesCount), "");
    QFileInfo fileInfo(path);
    if (fileInfo.exists()) {
        if (fileInfo.isFile()) {
            doScan(fileInfo.absoluteFilePath());
        } else {
            QDirIterator dirIterator(path, QDir::NoDotAndDotDot | QDir::AllEntries, QDirIterator::Subdirectories);
            while (dirIterator.hasNext()) {
                QString file = dirIterator.next();
                QFileInfo fileInfo(file);
                if (fileInfo.isDir()) {
                    continue;
                }
                doScan(file);
            }
        }
    }
    QThreadPool::globalInstance()->waitForDone();
    writeToClient(DONE, QString("done"), "");
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

void ClientConnection::writeToClient(ResponceTypes type, QString message, QString filePath = "") {
    QJsonObject root;
    root["message"] = message;

    switch (type) {
    case INIT:
        root["type"] = "init";
        break;
    case DONE:
        root["type"] = "done";
        break;
    case VERDICT:
        root["type"] = "verdict";
        root["filePath"] = filePath;
        break;
    default:
        break;
    }
    QString text = QString(QJsonDocument(root).toJson(QJsonDocument::Compact) + "\n");
    logger->writeLog(text);
//    client->write(text.toUtf8());
//    client->waitForBytesWritten();
}

void ClientConnection::doScan(QString filePath) {
    FileProcessor *processor = new FileProcessor(filePath, sigMap);
    processor->setAutoDelete(true);
    connect(processor, SIGNAL(sendVerdict(QString, QString)), this, SLOT(printVerdict(QString, QString)), Qt::DirectConnection);
    QThreadPool::globalInstance()->start(processor);
}

void ClientConnection::printVerdict(QString result, QString filePath) {
    qDebug() << filePath << ": " << result;
    writeToClient(VERDICT, result, filePath);
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

ClientConnection::~ClientConnection(){
    delete logger;
    delete client;
}
