#ifndef MAINWINDOWVIEW_H
#define MAINWINDOWVIEW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

#include "gameview.h"
#include "newgamedialog.h"
#include "gamemodel.h"
#include "tileview.h"
#include "dockwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *seedStatusLabel;
    QLabel *algoStatusLabel;
    QLabel *sizeStatusLabel;

    DockWindow *dockWindow;
    GameModel *gameModel;
    GameView *gameView;
protected:
    virtual void keyPressEvent(QKeyEvent *ev) override;
};


#endif // MAINWINDOWVIEW_H
