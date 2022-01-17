#include "tilemodel.h"
#include <climits>

QColor TileModel::ConnectedTileColor = QColor(0, 200, 0);
QColor TileModel::DetachedTileColor = Qt::blue;

TileModel::TileModel(QObject* parent)
    : QObject(parent),
      _nodes(QVector<bool>(4,false)),
      _color(Qt::blue),
      _angle(0),
      _clickable(true),
      _timer(new QTimer(this))
{
    setBgdColor(Qt::transparent);
    _timer->setInterval(7);
    connect(_timer, &QTimer::timeout, this, &TileModel::rotateTimeout);
}

TileModel::TileModel(const QVector<bool>& nodes, QObject *parent)
    : QObject(parent),
      _nodes(nodes),
      _color(Qt::blue),
      _angle(0),
      _clickable(true),
      _connected(false),
      _timer(new QTimer(this))
{
    setBgdColor(Qt::transparent);
    _timer->setInterval(7);
    connect(_timer, &QTimer::timeout, this, &TileModel::rotateTimeout);
}

void TileModel::connected(bool connectionState) {
    if (connectionState) {
        setColor(TileModel::ConnectedTileColor); // change tile color to green
        setClickState(false);
    }
    else {
        setColor(TileModel::DetachedTileColor);
//        setClickState(true);
    }
}

void TileModel::rotate(int n) {
    if(n == 0) return;
    n %= 4;
    for(int j = 1; j <= n; ++j){
        bool last =  _nodes[3];
//        for (int i = 3; i > 0; i--)
//            _nodes[i] = _nodes[i - 1];
//        _nodes[0] = last;
        _nodes[3] = _nodes[2];
        _nodes[2] = _nodes[1];
        _nodes[1] = _nodes[0];
        _nodes[0] = last;
        //  for animation   
    }
    if (_timer->isActive())
        _animationAngle += n*90;
    else
        _animationAngle = n * 90;
    _timer->start();
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
    emit bgcChanged();
}

void TileModel::rotateTimeout()
{
    _angle += 10;
    _animationAngle -= 10;
    emit tileChanged();
    if (_animationAngle == 0)
        _timer->stop();
}

TileModel::TileNames TileModel::typeByVector(const QVector<bool>& tile)
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

void TileModel::setNodes(const QVector<bool> &nodes)
{
    if (TileModel::typeByVector(nodes) != type())
        throw "This Tile is invalid!";
    int count = 0;
    QVector<bool> temp(_nodes);
    while (temp != nodes) {
        bool last = temp[3];
        for (int i = 3; i > 0; --i)
            temp[i] = temp[i - 1];
        temp[0] = last;
        ++count;
    }
    rotate(count);
}


