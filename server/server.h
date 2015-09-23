#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QThread>

class Connection;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    virtual ~Server();
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

};

#endif // SERVER_H