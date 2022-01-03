#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QFrame>
#include "tilemodel.h"

class TileView : public QFrame
{
    Q_OBJECT
private:
    TileModel *tile;
    QColor tileColor;
    QTimer *animationTimer; // for animation
//    QTimer *hintAnimationTimer; // for hint animation
    int animationAngele = 90;
    int rotateAngle = 0;
    bool canBeClicked = true;
//    double alphaValueF = 0;
public:
    explicit TileView(TileModel *tile, QColor color, QFrame *parent = nullptr);

    void rotateWithAnimation(int angle = 90);

    void adjustAngle();
    void setBackgroundColor(QColor color);

protected:
    void paintEvent(QPaintEvent*) override;
signals:
    void nodeChange(int times);
    void clicked();
//    void clickedWhileHint(TileModel *tileModel);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;

public slots:
//    void startHintAnimation();
//    void stopHintAnimation();
    void isConnected(bool connected);
    void setTileColor(QColor color);
private slots:
    void rotateTimeout(); // for rotate animation
//    void hintAnimationTimeout(); // for hint animation
};

#endif // TILEVIEW_H
