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
    const static size_t HINTLIMIT = 3;
private:
    Ui::DockWindow *ui;

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

};


#endif // DOCKWINDOW_H
