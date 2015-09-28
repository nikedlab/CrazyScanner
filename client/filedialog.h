#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class FileDialog;
}

class FileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileDialog(QWidget *parent = 0);
    ~FileDialog();
    void updateFileBrowser(QString path);

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_buttonBox_accepted();

    void on_treeView_doubleClicked(const QModelIndex &index);

signals:
    void acceptedClicked(QString str);

private:
    Ui::FileDialog *ui;
    QFileSystemModel *explorer;
    QString currentItemPath;
};

#endif // FILEDIALOG_H
