#include "filedialog.h"
#include "ui_filedialog.h"
#include <QSettings>
#include <QDebug>

FileDialog::FileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);

    QSettings settings ("NikedLab", "CrazyScanner");

    QString homePath = settings.value("extPath", QDir::homePath()).toString();

    explorer = new QFileSystemModel(this);
    explorer->setRootPath(homePath);
    explorer->setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    updateFileBrowser(homePath);

}

void FileDialog::updateFileBrowser(QString path)
{
    QTreeView *tree = ui->treeView;
    tree->setModel(explorer);
    tree->setRootIndex(explorer->index(path));
    tree->setUniformRowHeights(true);
    tree->setAnimated(true);
    tree->setSortingEnabled(true);
}

FileDialog::~FileDialog()
{
    delete ui;
}

void FileDialog::on_treeView_clicked(const QModelIndex &index)
{
    currentItemPath = explorer->fileInfo(index).absoluteFilePath();

}

void FileDialog::on_buttonBox_accepted()
{

    emit acceptedClicked(currentItemPath);
}
