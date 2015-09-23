#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QThread>
#include "filelogger.h"

class Connection;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    virtual ~Server();
    void startServer();
    FileLogger *logger;


protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

};

#endif // SERVER_H
