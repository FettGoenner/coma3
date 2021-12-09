#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionPause_triggered()
{
    QMessageBox::information(this, "Pause", "");
}


void MainWindow::on_actionReset_triggered()
{
    QMessageBox::information(this, "Reset", "");

}


void MainWindow::on_actionNew_Game_triggered()
{
    QMessageBox::information(this, "New Game", "");
}


void MainWindow::on_actionSave_Game_triggered()
{
    QMessageBox::information(this, "Save Game", "");
}


void MainWindow::on_actionHint_triggered()
{
    QMessageBox::information(this, "Hint", "");
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "help", "<b>ERLÄUTERUNG:</b><br>"


                                           "Ihre Aufgabe ist es, das defekte Netztwerk zu reparieren. "
                                           "Das ist so einfach! Drehen Sie die Netzwerkteile, indem Sie sie berühren. "
                                           "Tun Sie dies so, dass jedes Teil mit jedem anderen Teil verbunden ist."
                                           "Schritt für Schritt werden Sie alle Teile miteinander verbunden und ein Netzwerk erstellen, "
                                           "in dem alle Teile miteinander verbunden sind.");
   // <img src="file:///C:/Users/Tuya/OneDrive/Bilder/Screenshots/Screenshot (48).png">;

}


void MainWindow::on_actionAbout_At_triggered()
{
    QApplication::aboutQt();
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

