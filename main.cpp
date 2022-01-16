#include <QApplication>
#include <QStandardPaths>
#include <QDir>

#include "mainwindowview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationDomain("https://github.com/FettGoenner/coma3");
    app.setApplicationName("NetworkPuzzle");
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if (path.isEmpty()) qFatal("Cannot determine settings storage location");
    QDir d{path};
    if (d.mkpath(d.absolutePath()) && QDir::setCurrent(d.absolutePath()))
        qDebug() << "settings in" << QDir::currentPath();

    MainWindow w;
    w.show();
    return app.exec();
}
