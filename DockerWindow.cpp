#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTime>
#include <QString>
#include <QLabel>
#include "DockerWindow.h"


DockerWindow::DockerWindow(QWidget * parent)
{
    QPushButton * pause = new QPushButton("Pause") ;
    QPushButton * reset = new QPushButton("Reset") ;
    QPushButton * hint = new QPushButton("Hint") ;
    QPushButton * newg = new QPushButton("New Game") ;
    QPushButton * save = new QPushButton("Save Game") ;

    QTime time = QTime::currentTime() ;
    QString text = time.toString("mm:ss") ;
    QLabel * label = new QLabel(text) ;

    QVBoxLayout * layout = new QVBoxLayout(this) ;
    layout->addWidget(label) ;

    layout->addWidget(pause) ;
    layout->addWidget(reset) ;
    layout->addWidget(hint) ;
    layout->addWidget(newg) ;
    layout->addWidget(save) ;
}
