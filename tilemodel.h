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
    enum NodesSwitch{ON=true, OFF=false};
    explicit TileModel(QObject *parent = nullptr);

    bool north = TileModel::OFF;
    bool east = TileModel::OFF;
    bool south = TileModel::OFF;
    bool west = TileModel::OFF;
    int rotateAngle = 0; // the angle that this node already rotated

    QVector<bool> getNodeVector();

    int getAngle() const {
        return this->rotateAngle;
    }

    void rotate90(); // rotate the node 90 degrees clockwise

    void clearNodes() {
        this->north = TileModel::OFF;
        this->east = TileModel::OFF;
        this->south = TileModel::OFF;
        this->west = TileModel::OFF;
    }

    bool noNodes() const {
        return !(this->north || this->east || this->south || this->west);
    }

    QString getNodeString();


    virtual void setNodes(bool north, bool east, bool south, bool west) {
        this->clearNodes();
        if (north)
            this->north = TileModel::ON;
        if (east)
            this->east = TileModel::ON;
        if (south)
            this->south = TileModel::ON;
        if (west)
            this->west = TileModel::ON;
    }

    virtual void setNodes(const QVector<bool>& tile) {
        this->clearNodes();
        if (tile[TileModel::North])
            this->north = TileModel::ON;
        if (tile[TileModel::East])
            this->east = TileModel::ON;
        if (tile[TileModel::South])
            this->south = TileModel::ON;
        if (tile[TileModel::West])
            this->west = TileModel::ON;
    }

    virtual QString getTileType() {
        return "Tile";
    }


    static QString getNodeString(const QVector<bool>& tile);

    static QString getTileTypeByVector(const QVector<bool>& tile);

    static bool noNodes(const QVector<bool>& tile) {
                return !(tile[TileModel::North] ||
                tile[TileModel::East] ||
                tile[TileModel::South]||
                tile[TileModel::West]);
    }

    static bool has3Nodes(const QVector<bool>& tile) {
        return TileModel::countNodes(tile) == 3;
    }

    static size_t countNodes(const QVector<bool>& tile);

private:


public slots:
    void adjustNodes(int times = 1);

signals:
    void nodesChanged(); // if the nodes have changed
    void resetedTile();
    void rotatedByHint();
};

#endif // TILEMODEL_H
