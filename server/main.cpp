#include <QApplication>
#include "main.h"
#include "server.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server *server = new Server;
    server->startServer();

    /*QString path = "/home/nike/viruses";
    FileProcessor *processor = new FileProcessor;
    QFileInfo fileInfo(path);
    if (fileInfo.exists()) {
        if (fileInfo.isFile()) {
            QString result = processor->processFile(fileInfo.absoluteFilePath());
            qDebug() << result;
        } else {
            QDirIterator dirIterator(path, QDir::NoDotAndDotDot | QDir::AllEntries, QDirIterator::Subdirectories);
            while (dirIterator.hasNext()) {
                QString result = processor->processFile(dirIterator.next());
                qDebug() << result;
            }
        }
    }
    delete processor;*/

    return a.exec();
}
