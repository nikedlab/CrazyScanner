#include "filelogger.h"
#include <QFile>
#include <QTextStream>

FileLogger::FileLogger()
{

}

void FileLogger::writeLog(QString data) {
    QFile file("/tmp/server_log");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    out << QString(data + "\n");
    file.close();
}
