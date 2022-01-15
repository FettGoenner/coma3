#ifndef SAVELOADGAMEVIEW_H
#define SAVELOADGAMEVIEW_H

#include <QDialog>
#include <QTime>
#include <QDate>
#include <QMessageBox>
#include <QTime>
#include <QDate>

#include "saveloadgamemodel.h"
#include "networkpuzzlefile.h"

namespace Ui {
class SaveLoadGameDialog;
}

class SaveLoadGameView : public QDialog
{
    Q_OBJECT

public:
    enum SaveOrLoad{Save = 0, Load = 1};
    explicit SaveLoadGameView(SaveLoadGameModel *model, SaveLoadGameView::SaveOrLoad status = SaveLoadGameView::Save, QWidget *parent = nullptr);

    void loadFiles();
    void loadCurrentData();
    void setSelectionsToDefault();
    virtual void accept() override;
    ~SaveLoadGameView();

private:
    Ui::SaveLoadGameDialog *ui;
    SaveLoadGameModel *m_model;
    SaveLoadGameView::SaveOrLoad m_status;
    void clearLabels();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

signals:
    void deleteFile(size_t index);
    void saveBtnClicked(int index, QString fileName);
    void loadBtnClicked(int index);
};

#endif // SAVELOADGAMEVIEW_H
