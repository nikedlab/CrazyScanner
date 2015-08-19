#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"

#include <QDebug>
#include <QProcess>

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

void MainWindow::handle_file_item(QString str) {
    QFileInfo fileInfo(str);
    QString type = "Unknown";
    if (fileInfo.isDir() || fileInfo.isFile()) {
        type = fileInfo.isDir() ? "(Folder)" : "(File)";
    }

    ui->sourceType->setText(type);
    ui->sourcePath->setText(str);
    QApplication::processEvents();

    ui->sourcePath->setVisible(true);
    ui->sourceType->setVisible(true);
    ui->sourceLabel->setVisible(true);
    ui->startScan->setVisible(true);

    QProcess *process = new QProcess;
    process->start("server");

    Client *client = new Client;
    client->start();


}
