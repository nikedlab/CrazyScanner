#ifndef VIRUSLISTWINDOW_H
#define VIRUSLISTWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

namespace Ui {
class VirusListWindow;
}

class VirusListWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VirusListWindow(QList<QString> virusList, QWidget *parent = 0);
    ~VirusListWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::VirusListWindow *ui;
    QList<QString> virusList;
    QStringListModel *model;
};

#endif // VIRUSLISTWINDOW_H
