#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QThreadPool>
#include "fileprocessor.h"


class ClientConnection : public QObject
{
    Q_OBJECT



public:
    ClientConnection(int socketDescriptor, QObject *parent = 0);
    enum ResponceTypes
    {
        INIT,
        DONE
    };

public slots:
    void run();

private slots:
    void disconnected();
    void readyRead();
    void printVerdict(QByteArray verdict);


private:
    const int socketDescriptor;
    QTcpSocket *client;
    FileProcessor *processor;
    QMap<QString, QString> *prepareMap();
    QMap<QString, QString> *sigMap;
    void doScan(QString filePath, QThreadPool *pool);
    void writeToClient(ResponceTypes type, QString message);
    int getFilesCount(QString dirPath);
};

#endif // CLIENTCONNECTION_H
