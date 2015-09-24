#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProcess>
#include "filedialog.h"

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
    void processStarted();
    void processStartError(QProcess::ProcessError error);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

public slots:
    void killServer();
    void initProgressBar(int maxSize);
    void updateProgressBar(QString file, QString verdict);
    void compliteScan();

private:
    Ui::MainWindow *ui;
    FileDialog *fileDialog;
    QString path;
    QProcess *process;
};

#endif // MAINWINDOW_H
