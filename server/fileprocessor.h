#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QMap>
#include <QObject>
#include <QRunnable>

class FileProcessor : public QObject, public QRunnable  {
    Q_OBJECT

public:
    FileProcessor(QString filePath, QMap<QString, QString> *sigMap);
    virtual ~FileProcessor();
    void run();

private:
    QString filePath;
    QMap<QString, QString> *sigMap;
    void send(QString result, QString filePath);

signals:
    void sendVerdict(QString result, QString filePath);

};

#endif // FILEPROCESSOR_H
