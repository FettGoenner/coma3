#include "MainWindow.h"
#include "DockerWindow.h"
#include <QDockWidget>
#include <QMenuBar>
#include <QAction>
#include <QLabel>

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent)
{
    // Zentrales Widget setzen

    // Titel und minimale Breite/Höhe setzen
    setWindowTitle("Main Window") ;
    setMinimumWidth(400) ;
    setMinimumHeight(200) ;

    // Dock-Widget mit PushButton links anfügen
    QDockWidget * dockwidget = new QDockWidget ;
    dockwidget->setWidget(new DockerWindow(this)) ;
    addDockWidget(Qt::RightDockWidgetArea, dockwidget) ;

    // Menü zur MenuBar hinzufügen
    QMenu * menu = new QMenu("Menu", this) ;
    menuBar()->addMenu(menu) ;

    // eine Aktion "Connect" zum Menü hinzufügen
    /* QAction * action = new QAction("Choose", this) ; */
    /* connect(action, &QAction::triggered, hello, &Hello::openDialog) ; */
    /* menu->addAction(action) ; */
}
