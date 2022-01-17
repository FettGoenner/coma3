#ifndef TILEMODEL_H
#define TILEMODEL_H

#include <QObject>
#include <QColor>
#include <QDebug>
#include <QTimer>
#include <QVector>

class TileModel : public QObject
{
    Q_OBJECT

    QVector<bool> _nodes;    //  Vector representation of nodes {North,East,South,West}
    QColor _color;
    int _angle; // relative angle to init. state
    int _animationAngle;
    bool _clickable;
    bool _connected;
    QString _bgdColor = "";
    QTimer* _timer;  //  for animation control

public:
    static QColor ConnectedTileColor;
    static QColor DetachedTileColor;

    enum NodeState{ON=true, OFF=false};
    enum NodesEnum{North, East=1, South, West};
    enum TileNames{Tile = -1, CornerTile = 0, EndTile, JunctionTile, LineTile};

    explicit TileModel(QObject*);
    explicit TileModel(const QVector<bool>&, QObject *parent = nullptr);

    //  GETTERS
    static bool noNodes(const QVector<bool>& nodes)  { return !(nodes[0] || nodes[1] || nodes[2] || nodes[3]); }
    static bool has3Nodes(const QVector<bool>& tile) {
    return TileModel::countNodes(tile) == 3;
    }
    static size_t countNodes(const QVector<bool>& tile);
    QVector<bool> nodes()const{ return _nodes; }
    QColor color()const{ return _color; }
    int angle()const{ return _angle; }
    bool clickable() const{return _clickable; }

    QString bgdColor() const{ return _bgdColor; }
    QTimer* timer() const { return _timer; }

    virtual TileModel::TileNames type() const {
        return TileModel::Tile;
    };

    static TileModel::TileNames typeByVector(const QVector<bool>&);
    size_t countNodes() const;    // returns number of nodes

    //SETERS
    void setNodes(const QVector<bool> &nodes);   //  {north,east,south,west}
    void rotate(int n = 1); //  rotates tile n times
    void setClickState( bool clickState ){ _clickable = clickState; emit tileChanged(); }
    void setBgdColor( QColor);

protected:
signals:
    void tileChanged();
    void clicked(TileModel*);
    void resetedTile();
    void rotatedByHint();
    void bgcChanged();
private slots:
    void rotateTimeout(); // for rotate animation
public slots:
    void connected(bool);
    void setColor(QColor color){ _color = color; emit tileChanged(); }


};
#endif // TILEMODEL_H
