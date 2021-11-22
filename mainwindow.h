#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include "node.h"

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
    QVector<QVector<Node>> playGround;
    int totalSteps;
    QTimer *timer;
    bool gameStarted;
    int totalPlayTime;
signals:
    QString sendSteps(QString totalsteps);
public slots:
    void setSteps() {
        if (!gameStarted)
            gameStarted = true;
        ++totalSteps;
        emit sendSteps(QString::number(totalSteps));
    }
};
#endif // MAINWINDOW_H
