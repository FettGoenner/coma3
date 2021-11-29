#include "Line.h"
#include "Corner.h"
#include "Junction.h"
#include "End.h"
#include "MainWindow.h"
#include <QApplication>
#include <iostream>

using namespace std ;

int main (int argc, char * argv[])
{
    QApplication app(argc, argv) ;

    MainWindow window ;
    window.show() ;

    return app.exec() ;
}
