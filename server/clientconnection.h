#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QThreadPool>
#include "fileprocessor.h"
#include "filelogger.h"


class ClientConnection : public QObject
{
    Q_OBJECT



public:
    ClientConnection(int socketDescriptor, QObject *parent = 0);
    ~ClientConnection();
    FileLogger *logger;
    enum ResponceTypes
    {
        INIT,
        DONE,
        VERDICT
    };

public slots:
    void run();

private slots:
    void disconnected();
    void readyRead();
    void printVerdict(QString result, QString filePath);


private:
    const int socketDescriptor;
    QTcpSocket *client;
    FileProcessor *processor;
    QMap<QString, QString> *prepareMap();
    QMap<QString, QString> *sigMap;
    void doScan(QString filePath);
    void writeToClient(ResponceTypes type, QString message, QString filePath);
    int getFilesCount(QString dirPath);
};

#endif // CLIENTCONNECTION_H
