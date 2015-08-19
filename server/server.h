#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "fileprocessor.h"

class Connection;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    virtual ~Server();
    void startServer();
    //FileProcessor *processor;

private:
    QTcpSocket *client;


signals:
    void newConnection(Connection *connection);

public slots:
    void printVerdict(QByteArray verdict);

private slots:
    void readyRead();
    void disconnected();

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

};

#endif // SERVER_H
