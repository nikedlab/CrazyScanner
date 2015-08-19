#include "client.h"
#include <QHostAddress>

Client::Client(QObject *parent) :
    QObject(parent)
{
    connect(&client, SIGNAL(connected()), this, SLOT(sendRequest()));
}

Client::~Client(){
    client.close();
}

void Client::start() {
    QHostAddress host("localhost");
    client.connectToHost(host, 1024);
}

void Client::sendRequest() {
    client.write("Hell world!");
}
