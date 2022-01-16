#include "tilemodel.h"
#include <climits>

QColor TileModel::connectedTileColor = QColor(0, 200, 0);
QColor TileModel::detachedTileColor = Qt::blue;

TileModel::TileModel(QObject* parent)
    : QObject(parent),
      _nodes(QVector<bool>(4,false)),
      _color(Qt::blue),
      _angle(0),
      _clickable(true),
      _bgdColor(Qt::transparent),
      _timer(new QTimer(this))
{
    _timer->setInterval(3);
}

TileModel::TileModel(const QVector<bool>& nodes, QObject *parent)
    : QObject(parent),
      _nodes(nodes),
      _color(Qt::blue),
      _angle(0),
      _clickable(true),
      _connected(false),
      _bgdColor(Qt::transparent),
      _timer(new QTimer(this))
{
    _timer->setInterval(3);
}

void TileModel::connected(bool connectionState){
    if (connectionState){
        setColor(TileModel::connectedTileColor); // change tile color to green
        setClickState(false);
    }
    else {
        setColor(TileModel::detachedTileColor);
        setClickState(true);
    }
}
void TileModel::rotate(int n){
    if( n == 0) return;

    for(size_t j = 1; j<=n; ++j){
        bool last =  _nodes[3];
        for (int i = 3; i > 0; i--)
            _nodes[i] = _nodes[i - 1];
        _nodes[0] = last;
        //  for animation
        for(int i = 90; i>0;--i){
            _timer->start(3);
            ++_angle;
            emit tileChanged();
        }
        _angle &= 360;  // 0 <= _angle <= 360
    }
}

void TileModel::setBgdColor(QColor color)
{
    if (color == Qt::transparent) {
        setClickState(true);
        _bgdColor = "QWidget{"
                    "border:2px solid gray;"
                    "}";
    }
    else if (color == Qt::yellow) {
        setClickState(false);
        _bgdColor = "QWidget{"
                    "border:2px solid gray;"
                    "background-color: rgb(255, 255, 0);"
                    "}";
    }
    emit tileChanged();
}

int TileModel::typeByVector(const QVector<bool>& tile)
{
    if(tile.count(1) == 1) return TileModel::EndTile;
    else if(tile.count(1) == 3) return TileModel::JunctionTile;
    else if(tile.count(1) == 2)
    {

        if((tile[0] == 1 && tile[2] == 1) || (tile[1] == 1 && tile[3] == 1))
            return TileModel::LineTile;
        return TileModel::CornerTile;
    }
    return TileModel::Tile;
}

size_t TileModel::countNodes(const QVector<bool>& tile)
{
    int count = 0;
    for (const bool &node : tile)
        if (node)
            ++count;
    return count;
}


