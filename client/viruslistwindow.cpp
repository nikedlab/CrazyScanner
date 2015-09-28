#include "viruslistwindow.h"
#include "ui_viruslistwindow.h"

VirusListWindow::VirusListWindow(QList<QString> virusList, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VirusListWindow),
    virusList(virusList)
{
    ui->setupUi(this);
    setWindowTitle("Virus list");

    model = new QStringListModel(this);
    model->setStringList(virusList);

    ui->listView->setModel(model);
}

VirusListWindow::~VirusListWindow()
{
    delete ui;
}

void VirusListWindow::on_pushButton_clicked()
{
    close();
}
