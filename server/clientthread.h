#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "fileprocessor.h"


class ClientThread : public QThread
{

    Q_OBJECT

public:
    ClientThread(int socketDescriptor, QObject *parent);
    void run();

public slots:
    void disconnected();
    void readyRead();
    void printVerdict(QByteArray verdict);

private:
    int socketDescriptor;
    QTcpSocket *socket;
    FileProcessor *processor;
    QMap<QString, QString> *prepareMap();
    QMap<QString, QString> *sigMap;
};

#endif // CLIENTTHREAD_H
