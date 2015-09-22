#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include "fileprocessor.h"


class ClientConnection : public QObject
{

    Q_OBJECT

public:
    ClientConnection(int socketDescriptor, QObject *parent = 0);

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
};

#endif // CLIENTCONNECTION_H
