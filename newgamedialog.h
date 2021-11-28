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
    explicit NewGameDialog(QWidget *parent = nullptr, int row = 7, int col = 7);
    ~NewGameDialog();
    int rows;
    int columns;
    int seed;
private:
    Ui::NewGameDialog *ui;
    int getRandomValue();
};

#endif // NEWGAMEDIALOG_H
