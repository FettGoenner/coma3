#ifndef TILEMODEL_H
#define TILEMODEL_H

#include <QVector>
#include <QDebug>
#include <QTimer>
#include <QObject>

class TileModel : public QObject
{
    Q_OBJECT
public:
    enum NodesEnum{North, East=1, South, West};
    explicit TileModel(QObject *parent = nullptr);

    QVector<bool> nodes = {0, 0, 0, 0}; // shows where the ways are {top, right, bottom, left}

    int rotateAngle = 0; // the angle that this node already rotated

    QString getNodeString();
    int getAngel() const {
        return this->rotateAngle;
    }
    void rotate90(); // rotate the node 90 degrees clockwise

    void clearNodes() {
        for (int i = 0; i < 4; ++i)
            this->nodes[i] = false;
    }

    bool noNodes() const {
        return !(this->nodes[0] || this->nodes[1] || this->nodes[2] || this->nodes[3]);
    }
    bool sameNodes(QVector<int> nodesTemp) const {
        if (nodesTemp.size() != 4)
            return false;
        return this->nodes[0] == nodesTemp[0] &&
               this->nodes[1] == nodesTemp[1] &&
               this->nodes[2] == nodesTemp[2] &&
               this->nodes[3] == nodesTemp[3];
    }

    virtual void setNodes(QString nodes) {
        this->clearNodes();
        for (const auto &c : nodes)
            this->nodes[c.digitValue()] = true;
        emit this->nodesChanged();
        return;
    }

    virtual QString getTileType() {
        return "Tile";
    }

private:


public slots:
    void adjustNodes();
    void adjustNodes(int times);


signals:
    void nodesChanged(); // if the nodes have changed
    void startHint();
    void endHint();
    void resetTile();
};

#endif // TILEMODEL_H
