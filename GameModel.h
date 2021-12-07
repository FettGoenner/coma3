#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "TileModel.h"
#include <stack>

class GameModel
    : public QObject
{
    Q_OBJECT

/*** private attributes ***/

    // GameModel grid dimensions
    size_t  _mRows ; // Row
    size_t  _nCols ; // Column

    // values of start Tile
    size_t _sRow ;
    size_t _sCol ;

    // the grid of pointers
    TileModel const* _grid [_mRows] [_nCols] {} ;

public:

/*** constructors ***/

    GameModel()
        : _mRows(4),
          _nCols(4),
          _sRow(rand() % _mRows),
          _sCol(rand() % _nCols)
    {
        // write empty Tiles to grid
        for (size_t m = 0 ; m < _mRows ; ++m)
            for (size_t n = 0 ; n < _nCols ; ++n)
                TileAt(m, n) = new TileModel ;

        std::stack<std::vector<size_t>> pathStack ;
        pathStack.push(std::vector<size_t>(_sRow, _sCol)) ;

        // pathing a way
        while (!pathStack.empty())
        {
            size_t m = pathStack.top()[0] ;
            size_t n = pathStack.top()[1] ;

            std::vector<bool> validNodes ;
            // north valid?
            if (m > 0 && TileAt(m-1, n).nNodes() == 0)
                validNodes.pushBack(&TileAt(m,n).north) ;
            // south valid?
            if (m < _mRows && TileAt(m+1, n).nNodes() == 0)
                validNodes.pushBack(&TileAt(m,n).south) ;
            // east valid?
            if (n > 0 && TileAt(m, n-1).nNodes() == 0)
                validNodes.pushBack(&TileAt(m,n).east) ;
            // west valid?
            if (n < _nCols && TileAt(m, n+1).nNodes() == 0)
                validNodes.pushBack(&TileAt(m,n).west) ;

            if (validNodes.empty())
                cStack.popBack() ;
            else
                node = rand() % validNodes.size() ;
            validNodes[node] = true ;
        }
    }

    GameModel(TileModel**, size_t dX, size_t dY, size_t sX, size_t sY) ;

    // destructor
    ~GameModel()
    {
        for (size_t i ; i < _mRows ; ++i)
            for (size_t j ; j < _nCols ; ++j)
                if (_grid[i][j] != nullptr)
                    delete _grid[i][j] ;
    }

/*** getters ***/

    // checks if puzzle solved
    bool solved() ;

/*** setters ***/

    // Tile at coordinates x,y (or valid modulo)
    TileModel const*& TileAt(size_t x, size_t y)
    {
        // ensure valid values 
        x %= _mRows ;
        y %= _nCols ;

        return _grid[x][y] ;
    }
} ;

#endif // GAMEMODEL_H
