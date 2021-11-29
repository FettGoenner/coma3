#include "mainwindow.h"
#include "choosedialog.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

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


void MainWindow::on_actionpause_triggered()
{
    QMessageBox::information(this, "title", "pause");
}


void MainWindow::on_actionopen_triggered()
{
    QMessageBox::information(this, "title", "new game");
}


void MainWindow::on_actionexit_triggered()
{
    QMessageBox::information(this, "title", "reset");
}


void MainWindow::on_actionhint_triggered()
{
    QMessageBox::information(this, "title", "hint");
}


void MainWindow::on_actionhelp_triggered()
{
    QMessageBox::information(this, "help", "<b>ERLÄUTERUNG:</b>  "
                                           "Ihre Aufgabe ist es, das defekte Netztwerk zu reparieren. "
                                           "Das ist so einfach! Drehen Sie die Netzwerkteile, indem Sie sie berühren. "
                                           "Tun Sie dies so, dass jedes Teil mit jedem anderen Teil verbunden ist."
                                           "Schritt für Schritt werden Sie alle Teile miteinander verbunden und ein Netzwerk erstellen, "
                                           "in dem alle Teile miteinander verbunden sind.");


}


void MainWindow::on_actionsave_game_triggered()
{
    QMessageBox::information(this, "title", "save game");


}


void MainWindow::on_actionexit_2_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionabout_at_triggered()
{
    QApplication::aboutQt();
}

