#include "fileprocessor.h"
#include <QFile>
#include <QDir>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <QByteArray>
#include <QStringMatcher>
#include <QJsonObject>
#include <QJsonDocument>


FileProcessor::FileProcessor(QString filePath, QMap<QString, QString> *sigMap)
{
    this->sigMap = sigMap;
    this->filePath = filePath;
}

void FileProcessor::run() {
    QFile target(filePath);

    if (target.size() > 157286400) { // 150Mb
        toJson("Error! To big file!", filePath);
        return;
    }
    if(target.open(QIODevice::ReadOnly)) {
        QByteArray *bytes = new QByteArray(target.readAll());
        QString hexContent = bytes->toHex();
        foreach (QString key, sigMap->keys()) {
            QStringMatcher matcher(key, Qt::CaseInsensitive);
            int index = matcher.indexIn(hexContent);
            if (index != -1) {
                toJson(sigMap->value(key), filePath);
                return;
            }
        }
    } else {
        toJson("Error! Can't open target file", filePath);
        return;
    }
    toJson("Clear", filePath);
}

void FileProcessor::toJson(QString result, QString filePath) {
    QJsonObject root;
    root["type"] = "verdict";
    root["filePath"] = filePath;
    root["message"] = result;
    QByteArray verdict = QJsonDocument(root).toJson(QJsonDocument::Compact);
    emit sendVerdict(verdict);
}

FileProcessor::~FileProcessor() {

}
