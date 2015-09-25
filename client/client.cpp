#include "client.h"
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>

Client::Client(QString request) :
    QObject()
{
    this->request = request;
    client = new QTcpSocket(this);
    connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(sendRequest(QAbstractSocket::SocketState)));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError(QAbstractSocket::SocketError)));
    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

Client::~Client(){
    delete client;
}

void Client::start() {
    QHostAddress host(QHostAddress::LocalHost);
    client->connectToHost(host, 1024);
}

void Client::sendRequest(QAbstractSocket::SocketState state) {
    qDebug() << "Connection state: " << state;
    if (state == QAbstractSocket::ConnectedState) {
        client->write(request.toUtf8());
        client->waitForBytesWritten();
    }
}

void Client::readyRead() {
        QJsonDocument json;
        QJsonObject jsonObject;

        QByteArray line = client->readLine().trimmed();
        qDebug() << line;
        json = QJsonDocument::fromJson(line);
        jsonObject = json.object();
        QString type = jsonObject["type"].toString();
        if (type == "done") {
            client->close();
            emit compliteScan();
        } else if (type == "init") {
            int count = jsonObject["message"].toInt();
            emit initProgressBar(count);
        } else if (type == "verdict") {
            QString verdict = jsonObject["message"].toString();
            QString filePath = jsonObject["filePath"].toString();
            emit updateProgressBar(filePath, verdict);
        }

}

void Client::connectError(QAbstractSocket::SocketError socketError) {
    qDebug() << "Connect error: " << socketError;
    emit compliteScan();
}
