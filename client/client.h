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
    void initProgressBar(int maxSize);
    void updateProgressBar(QString file, QString verdict);
    void compliteScan();

public slots:
    void sendRequest(QAbstractSocket::SocketState state);
    void connectError(QAbstractSocket::SocketError socketError);
    void start();
    void readyRead();

private:
    QTcpSocket *client;
    QString request;

};

#endif // CLIENT_H
