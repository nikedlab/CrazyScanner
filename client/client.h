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
    void sendRequest();
    void connectError(QAbstractSocket::SocketError socketError);
    void start();

private:
    QTcpSocket *client;
    QString request;

};

#endif // CLIENT_H
