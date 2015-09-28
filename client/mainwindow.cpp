#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include "viruslistwindow.h"

#include <QDebug>
#include <QThread>
#include <QPalette>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sourcePath->setVisible(false);
    ui->sourceType->setVisible(false);
    ui->sourceLabel->setVisible(false);
    ui->startScan->setVisible(false);
    ui->progressBar->setVisible(false);

    ui->infectedCountLabel->setVisible(false);
    ui->infectedCount->setVisible(false);

    ui->btnShowViruses->setVisible(false);

    QPalette palette = ui->infectedCountLabel->palette();
    palette.setColor(ui->infectedCountLabel->foregroundRole(), Qt::red);
    ui->infectedCountLabel->setPalette(palette);

    palette = ui->infectedCount->palette();
    palette.setColor(ui->infectedCount->foregroundRole(), Qt::red);
    ui->infectedCount->setPalette(palette);

    ui->tableWidget->setColumnCount(2);
    QStringList titles;
    titles << "File" << "Verdict";
    ui->tableWidget->setHorizontalHeaderLabels(titles);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    fileDialog = new FileDialog;

    connect(fileDialog, SIGNAL(acceptedClicked(QString)), this, SLOT(handle_file_item(QString)));

    process = new QProcess;
    connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processStartError(QProcess::ProcessError)));
    process->start("./server");
    bool startServerResult = process->waitForStarted();
    qDebug() << "Server star attempt: " << startServerResult;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    qDebug() << "closeEvent";
    process->kill();
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
    while (ui->tableWidget->rowCount() > 0) {
        ui->tableWidget->removeRow(0);
    }
    startClient();

}

void MainWindow::startClient() {
        QThread *thread = new QThread();
        Client *client = new Client(path);
        client->moveToThread(thread);

        connect(thread, SIGNAL(started()), client, SLOT(start()));
        connect(client, SIGNAL(initProgressBar(int)), this, SLOT(initProgressBar(int)));
        connect(client, SIGNAL(updateProgressBar(QString, QString)), this, SLOT(updateProgressBar(QString, QString)),  Qt::UniqueConnection);
        connect(client, SIGNAL(compliteScan()), this, SLOT(compliteScan()));

        connect(client, SIGNAL (compliteScan()), thread, SLOT (quit()));
        connect(client, SIGNAL (compliteScan()), client, SLOT (deleteLater()));
        connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));

        thread->start();
}

void MainWindow::processStartError(QProcess::ProcessError error) {
    qDebug() << "Server start error: " << error;
}

void MainWindow::initProgressBar(int maxFilesSize) {
    ui->progressBar->setRange(0, maxFilesSize);
    ui->progressBar->setVisible(true);
    connect(this, SIGNAL(newProgressValue(int)), ui->progressBar, SLOT(setValue(int)));

    ui->infectedCountLabel->setVisible(true);
    ui->infectedCount->setVisible(true);
    connect(this, SIGNAL(signalToUpdateInfectedCount(int)), ui->infectedCount, SLOT(setNum(int)));
    emit signalToUpdateInfectedCount(0);
    currentCount = 0;
    infectedFiles.clear();

    ui->btnShowViruses->setVisible(false);
}

int MainWindow::updateInfectedCount() {
    return ++currentCount;
}

void MainWindow::updateProgressBar(QString file, QString verdict) {
    int currentValue = ui->progressBar->value();
    emit newProgressValue(currentValue + 1);

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    QTableWidgetItem *fileCell = new QTableWidgetItem(file);
    fileCell->setFlags(fileCell->flags() ^ Qt::ItemIsEditable);
    fileCell->setFlags(fileCell->flags() ^ Qt::ItemIsSelectable);

    QTableWidgetItem *verdictCell = new QTableWidgetItem(verdict);
    verdictCell->setFlags(verdictCell->flags() ^ Qt::ItemIsEditable);
    verdictCell->setFlags(verdictCell->flags() ^ Qt::ItemIsSelectable);


    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, fileCell);
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, verdictCell);

    ui->tableWidget->scrollToBottom();

    if(verdict.startsWith("{")) {
        emit signalToUpdateInfectedCount(updateInfectedCount());
        infectedFiles.append(file);
    }
}

void MainWindow::compliteScan() {
    ui->progressBar->setVisible(false);
    qDebug() << "compliteScan";
    if (!infectedFiles.empty()) {
        ui->btnShowViruses->setVisible(true);
    }
}

void MainWindow::on_btnShowViruses_clicked()
{
    VirusListWindow *virusListWindow = new VirusListWindow(infectedFiles, this);
    virusListWindow->show();
    virusListWindow->setGeometry(virusListWindow->geometry());
}
