#ifndef TILE_H
#define TILE_H

#include <QFrame>
#include <QVector>
#include <QDebug>
class Tile : public QFrame
{
    Q_OBJECT
public:
    enum NodesEnum{North, East=1, South, West};
    explicit Tile(QFrame *parent = nullptr);
    void rotate90(); // rotate the node 90 degrees clockwise
    QVector<bool> nodes = {0, 0, 0, 0}; // shows where the ways are {top, right, bottom, left}
    QColor nodeColor;
    int rotateAngle = 0; // the angle that this node already rotated
    QString getNodeString();
    void clearNodes() {
        for (int i = 0; i < 4; ++i)
            this->nodes[i] = false;
    }
    virtual void setNodes(QString nodes) {
        return;
    };

    static bool noNodes(QVector<bool> nodes) {
        return !(nodes[0] || nodes[1] || nodes[2] || nodes[3]);
    }
    static int getNode(int node) {
        int ans;
        switch (node) {
            case North:
                ans = South;
                break;
            case East:
                ans = West;
                break;
            case South:
                ans = North;
                break;
            case West:
                ans = East;
                break;
        }
        return ans;
    }
protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
private:

signals:
    void clicked();

};

#endif // TILE_H
