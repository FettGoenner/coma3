#ifndef TILEMODEL_H
#define TILEMODEL_H

#include <QObject>
#include <QColor>
#include <QDebug>
#include <QTimer>
#include <QVector>
#include <QMouseEvent>
#include <QWidget>

class TileModel : public QObject
{

    Q_OBJECT


    QVector<bool> _nodes;    //  Vector representation of nodes {North,East,South,West}
    QColor _color;
    int _angle; // relative angle to init. state
    bool _clickable;
    bool _connected;
    QString _bgdColor;
    QTimer* _timer;  //  for animation control

public:
    static QColor connectedTileColor;
    static QColor detachedTileColor;
    enum NodeState{ON=true,OFF=false};
    enum NodesEnum{North,East=1,South,West};
    enum TileNames{LineTile,CornerTile,JunctionTile,EndTile,Tile};
    explicit TileModel(QObject*);
    explicit TileModel( const QVector<bool>&, QObject *parent = nullptr );

    //  GETTERS
    static bool noNodes(const QVector<bool>& nodes)  { return !(nodes[0] || nodes[1] || nodes[2] || nodes[3]); }
    static bool has3Nodes(const QVector<bool>& tile) {
    return TileModel::countNodes(tile) == 3;
    }
    static size_t countNodes(const QVector<bool>& tile);
    QVector<bool>& nodes(){ return _nodes; } const
    QColor& color(){ return _color; } const
    int& angle(){ return _angle; } const
    bool& clickable(){return _clickable; } const

    QString& bgdColor(){ return _bgdColor; } const
    QTimer* timer(){ return _timer; } const
    virtual int type() = 0;
    static int typeByVector(const QVector<bool>&);
    size_t countNodes();    // returns number of nodes

    //SETERS
    void setNodes(QVector<bool> nodes){ _nodes = nodes; emit tileChanged(); }   //  {north,east,south,west}
    void rotate( int n = 1 ); //  rotates tile n times
    void setClickState( bool clickState ){ _clickable = clickState; emit tileChanged(); }
    void setBgdColor( QColor);

protected:
signals:
    void tileChanged();
    void clicked(TileModel*);
    void resetedTile();
    void rotatedByHint();

public slots:
    void connected(bool);
    void setColor( QColor color ){ _color = color; emit tileChanged(); }

};
#endif // TILEMODEL_H
