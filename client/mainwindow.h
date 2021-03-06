#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProcess>
#include "filedialog.h"
#include "viruslistwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void handle_file_item(QString path);
    void on_startScan_clicked();
    void processStartError(QProcess::ProcessError error);
    int updateInfectedCount();

    void on_btnShowViruses_clicked();

public slots:
    void initProgressBar(int maxFilesSize);
    void updateProgressBar(QString file, QString verdict);
    void compliteScan();

signals:
    void newProgressValue(int value);
    void signalToUpdateInfectedCount(int value);

private:
    Ui::MainWindow *ui;
    FileDialog *fileDialog;
    QString path;
    QProcess *process;
    void startClient();
    int currentCount = 0;
    QList<QString> infectedFiles;
    VirusListWindow *virusListWindow;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
