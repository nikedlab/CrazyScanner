#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QString request);
    ~Client();

signals:
    void killServer();

public slots:
    void sendRequest();
    void connectError(QAbstractSocket::SocketError socketError);
    void start();

private:
    QTcpSocket client;
    QString request;

};

#endif // CLIENT_H
