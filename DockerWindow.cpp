#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMainWindow>
#include "DockerWindow.h"


DockerWindow::DockerWindow(QWidget * parent)
{
    QPushButton * save = new QPushButton("save") ;

    QVBoxLayout * layout = new QVBoxLayout(this) ;
    layout->addWidget(save) ;
}
