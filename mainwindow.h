#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionPause_triggered();

    void on_actionReset_triggered();

    void on_actionNew_Game_triggered();

    void on_actionSave_Game_triggered();

    void on_actionHint_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_At_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
