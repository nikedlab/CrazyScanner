#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"

#include <QDebug>
#include <QThread>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sourcePath->setVisible(false);
    ui->sourceType->setVisible(false);
    ui->sourceLabel->setVisible(false);
    ui->startScan->setVisible(false);

    fileDialog = new FileDialog;

    connect(fileDialog, SIGNAL(acceptedClicked(QString)), this, SLOT(handle_file_item(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    fileDialog->show();

}

void MainWindow::handle_file_item(QString path) {
    this->path = path;
    qDebug() << "Selected: " + path;
    QFileInfo fileInfo(path);
    QString type = "Unknown";
    if (fileInfo.isDir() || fileInfo.isFile()) {
        type = fileInfo.isDir() ? "(Folder)" : "(File)";
    }

    ui->sourceType->setText(type);
    ui->sourcePath->setText(path);
    QApplication::processEvents();

    ui->sourcePath->setVisible(true);
    ui->sourceType->setVisible(true);
    ui->sourceLabel->setVisible(true);
    ui->startScan->setVisible(true);
}

void MainWindow::on_startScan_clicked()
{
    process = new QProcess;
    connect(process, SIGNAL(started()), this, SLOT(processStarted()));
    connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processStartError(QProcess::ProcessError)));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
    process->start("./server");
    bool startServerResult = process->waitForStarted();
    qDebug() << "Server star attempt: " << startServerResult;
}

void MainWindow::processStarted() {
    qDebug() << "Server started";
    QThread *thread = new QThread();
    Client *client = new Client(path);
    client->moveToThread(thread);
    connect(thread, SIGNAL(started()), client, SLOT(start()), Qt::DirectConnection);
    connect(client, SIGNAL(killServer()), this, SLOT(killServer()), Qt::DirectConnection);
    thread->start();
}

void MainWindow::killServer(){
    process->kill();
}

void MainWindow::processStartError(QProcess::ProcessError error) {
    qDebug() << "Server start error: " << error;
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    qDebug() << "Server process finished! Exit code: " << exitCode << " Exit status: " << exitStatus;
}
