#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    void start();

signals:

public slots:
    void sendRequest();

private:
    QTcpSocket client;

};

#endif // CLIENT_H
