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
    ClientConnection(int socketDescriptor);
    ~ClientConnection();
    FileLogger *logger;


public slots:
    void run();

private slots:
    void disconnected();
    void readyRead();
    void printVerdict(QString result, QString filePath);


private:
    enum ResponceTypes
    {
        INIT,
        DONE,
        VERDICT
    };
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
