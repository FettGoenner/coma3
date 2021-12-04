#ifndef TILE_H
#define TILE_H

#include <QFrame>
#include <QVector>
#include <QDebug>
#include <QTimer>

class Tile : public QFrame
{
    Q_OBJECT
public:
    enum NodesEnum{North, East=1, South, West};
    explicit Tile(QFrame *parent = nullptr);

    QVector<bool> nodes = {0, 0, 0, 0}; // shows where the ways are {top, right, bottom, left}
    QColor nodeColor;
    int rotateAngle = 0; // the angle that this node already rotated

    QString getNodeString();
    void rotate90(); // rotate the node 90 degrees clockwise
    void rotateWithAnimation(int angle = 90);

    void clearNodes() {
        for (int i = 0; i < 4; ++i)
            this->nodes[i] = false;
    }
    virtual void setNodes(QString nodes) {
        this->clearNodes();
        for (const auto &c : nodes)
            this->nodes[c.digitValue()] = true;

        return;
    };
private:
    QTimer *animationTimer; // for animation
    int animationAngele = 90;

    void adjustNodes();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
signals:
    void clicked();
private slots:
    void rotateTimeout();
};

#endif // TILE_H
