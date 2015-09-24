#include "client.h"
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>

Client::Client(QString request, QObject *parent) :
    QObject(parent)
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
    client.write(request.toUtf8());
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
            if (type == "done") {
                emit compliteScan();
                emit killServer();
            } else if (type == "init") {
                int count = jsonObject["message"].toInt();
                emit initProgressBar(count);
            } else if (type == "verdict") {
                QString verdict = jsonObject["message"].toString();
                QString filePath = jsonObject["filePath"].toString();
                emit updateProgressBar(filePath, verdict);
            }
        }
    }
}

void Client::connectError(QAbstractSocket::SocketError socketError) {
    qDebug() << "Connect error: " << socketError;
    emit killServer();
}
