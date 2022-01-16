#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QFrame>
#include "tilemodel.h"

class TileView : public QFrame
{
    Q_OBJECT
private:
    TileModel *m_model;
    QColor tileColor;
    QTimer *animationTimer; // for animation

    int animationAngle = 90;
    int rotateAngle = 0;

public:
    explicit TileView(TileModel *tile, QColor color, QFrame *parent = nullptr);

    void rotateWithAnimation(int angle = 90);
    bool canBeClicked = true;
    void adjustAngle();
    void setBackgroundColor(QColor color);

protected:
    void paintEvent(QPaintEvent*) override;
signals:
    void nodeChange(int times);
    void clicked();

public slots:
    void isConnected(bool connected);
    void setTileColor(QColor color);
private slots:
    void rotateTimeout(); // for rotate animation

};

#endif // TILEVIEW_H
