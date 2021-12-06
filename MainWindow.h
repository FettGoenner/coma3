#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenu>

class MainWindow
    : public QMainWindow
{
    Q_OBJECT

    // Menü-Attribute
    QMenu * fileMenu ;

    // Action-Attribute
    QAction * pauseAct ;
    QAction * resetAct ;
    QAction * newGameAct ;
    QAction * saveGameAct ;
    QAction * hintAct ;
    QAction * aboutAct ;
    QAction * exitAct ;

    // Menü-Methoden
    void createMenus() ;
    void createActions() ;


public:
/*** öffentliche Attribute ***/

/*** Konstruktoren ***/
    MainWindow(QWidget * parent = Q_NULLPTR) ;

private slots:
    void pause() ;
    void reset() ;
    void newGame() ;
    void saveGame() ;
    void hint() ;
    void about() ;
    void exit() ;
} ;

#endif // MAIN_WINDOW_H
