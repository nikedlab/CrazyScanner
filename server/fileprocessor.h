#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QMap>
#include <QObject>
#include <QRunnable>

class FileProcessor : public QObject, public QRunnable  {
    Q_OBJECT

public:
    FileProcessor(QObject *parent = 0);
    virtual ~FileProcessor();
    void processFile();
    QString filePath;

protected:
    void run();

private:
    void prepareMap();
    QMap<QString, QString> *sigMap;
    void toJson(QString result, QString filePath);

signals:
    void sendVerdict(QByteArray verdict);

};

#endif // FILEPROCESSOR_H
