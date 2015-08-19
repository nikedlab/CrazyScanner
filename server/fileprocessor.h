#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QMap>
#include <QObject>

class FileProcessor : public QObject
{
    Q_OBJECT

public:
    FileProcessor(QObject *parent = 0);
    virtual ~FileProcessor();
    void processFile(QString filePath);


private:
    void prepareMap();
    QMap<QString, QString> *sigMap;
    void toJson(QString result, QString filePath);

signals:
    void sendVerdict(QByteArray verdict);

};

#endif // FILEPROCESSOR_H
