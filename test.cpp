#include <QApplication>
#include <QLabel>
using namespace std ;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QLabel hello("Hello woow!");

    hello.show();
    return app.exec();
}
