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
    explicit NewGameDialog(size_t size = 7, QString algoType = "Depth", QWidget *parent = nullptr);
    ~NewGameDialog();
    size_t getSize();
    size_t getSeed();
    QString getAlgoType();
private:
    size_t _DIM;
    size_t seed;
    QString algoType;
    Ui::NewGameDialog *ui;
    int getRandomValue();
};

#endif // NEWGAMEDIALOG_H
