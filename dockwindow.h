#ifndef DOCKWINDOW_H
#define DOCKWINDOW_H

#include <QDockWidget>

namespace Ui {
class DockWindow;
}

class DockWindow : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockWindow(QWidget *parent = nullptr);
    ~DockWindow();
    const static size_t HINTLIMIT = 3; // Hint limit
private:
    Ui::DockWindow *ui;
    size_t hintCount = 0;

    void resetHint();
    void setHintBtnText();
signals:
    void changedGameStarted(bool started);
    void clickedSolutionBtn();
    void clickedHintBtn();
    void clickedResetBtn();
    void clickedNewGameBtn();

public slots:
    void gameStarted();
    void setStep(QString steps);
    void setTime(QString totalTime);
    void newGameStarted();
    void setHintCount();
};


#endif // DOCKWINDOW_H
