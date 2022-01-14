#ifndef SAVEGAMEVIEW_H
#define SAVEGAMEVIEW_H

#include <QDialog>
#include <QTime>
#include <QDate>

#include "savegamemodel.h"

namespace Ui {
class SaveGameDialog;
}

class SaveGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveGameDialog(QWidget *parent = nullptr);
    ~SaveGameDialog();

private:
    Ui::SaveGameDialog *ui;
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // SAVEGAMEVIEW_H
