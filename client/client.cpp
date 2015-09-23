#include "client.h"
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>

Client::Client(QString request) :
    QObject()
{
    connect(&client, SIGNAL(connected()), this, SLOT(sendRequest()));
    connect(&client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError(QAbstractSocket::SocketError)));
    this->request = request;

}

Client::~Client(){
    client.close();
}

void Client::start() {
    QHostAddress host(QHostAddress::LocalHost);
    client.connectToHost(host, 1024);
}

void Client::sendRequest() {
    qDebug() << "Request: " << request;
    client.write(request.toLatin1());
    client.waitForBytesWritten();
    client.waitForReadyRead();

    QJsonDocument json;
    QJsonObject jsonObject;
    forever {
        if (client.canReadLine()) {
            QByteArray line = client.readLine().trimmed();
            json = QJsonDocument::fromJson(line);
            jsonObject = json.object();
            QString type = jsonObject["type"].toString();
            qDebug() << "Type: " << type;
            if (type == "done") {
                emit killServer();
            }
        }
    }
}

void Client::connectError(QAbstractSocket::SocketError socketError) {
    qDebug() << "Connect error: " << socketError;
    emit killServer();
}
