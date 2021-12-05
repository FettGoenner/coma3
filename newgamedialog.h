#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QDialog>

namespace Ui {
class NewGameDialog;
}

class NewGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGameDialog(int row = 7, int col = 7, QString algoType = "Depth", QWidget *parent = nullptr);
    ~NewGameDialog();
    int rows;
    int columns;
    int seed;
    QString algoType;
private:
    Ui::NewGameDialog *ui;
    int getRandomValue();
};

#endif // NEWGAMEDIALOG_H
