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
    int animationAngele = 90;
    int rotateAngle = 0;
public:
    explicit TileView(TileModel *tile, QColor color, QFrame *parent = nullptr);

    void rotate90(); // rotate the node 90 degrees clockwise
    void rotateWithAnimation(int angle = 90);
    void adjustAngle();
protected:
    void paintEvent(QPaintEvent*) override;
signals:
    void nodeChange(int times);
    void clicked();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;

private slots:
    void rotateTimeout();
};

#endif // TILEVIEW_H
