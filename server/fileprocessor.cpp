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

FileProcessor::FileProcessor(QObject *parent)
{
    sigMap = new QMap<QString, QString>;
    prepareMap();
}

void FileProcessor::prepareMap() {
    QFile baseFile(":/av/base/base.txt");
    if (baseFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&baseFile);
        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(".");
            QString uuid = parts.takeAt(1);
            sigMap->insert(parts.takeAt(0), uuid);
        }
        baseFile.close();
    } else {
        qDebug() << "Error open base.txt";
    }
}

void FileProcessor::processFile(QString filePath) {
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
    root["filePath"] = filePath;
    root["message"] = result;
    QByteArray verdict = QJsonDocument(root).toJson(QJsonDocument::Compact);
    emit sendVerdict(verdict);
}

FileProcessor::~FileProcessor() {
    delete sigMap;
}
