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
private:
    Ui::DockWindow *ui;

    void setPauseBtnText(QString text = "Pause");

signals:
    void changedGameStarted(bool started);
    void clickedSolutionBtn();
    void clickedHintBtn();
    void clickedResetBtn();
    void clickedNewGameBtn();
    void hintBtnEnabledChanged(bool enabled);
    void solutionBtnEnabledChanged(bool enabled);
    void pauseBtnEnabledChanged(bool enabled);
    void pauseBtnTextChanged(QString text);

public slots:
    void gameStarted();
    void setStep(QString steps);
    void setTime(QString totalTime);
    void newGameStarted();
    void clickPauseBtn();
    void clickHintBtn();
    void clickSolutionBtn();
    void clickNewGameBtn();
    void clickResetBtn();
    void setHintBtnEnabled(bool enabled);
    void setSolutionBtnEnabled(bool enabled);
    void setPauseBtnEnabled(bool enabled);
    void setHintBtnText(int remaining);
};


#endif // DOCKWINDOW_H
