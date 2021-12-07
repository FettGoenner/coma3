#include "TileModel.h"
#include "GameModelModel.h"

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <stack>

// CONSTRUCTORS
GameModel::GameModel()
    : _DIMX(),
      _DIMY(),
      _startTile(),
      _field()
{ } ;

GameModel::GameModel(TileModel** initBoard, int startX, int startY, size_t dimX, size_t dimY)
    : _DIMX(dimX),
      _DIMY(dimY),
      _startTile(dimX,dimY)
{
    int max = 3; // upper bound amount of rotations per cell

    // RANDOMIZE TILES
    for (size_t row = 1; row <= _DIMX; ++row)
    {
        for (size_t col = 1; col <= _DIMY; ++col)
        {
            int rotCount = rand() % max;

            for (int cnt = 0; cnt < rotCount; ++cnt) {
                initBoard[row-1][col-1].rotate();
                incrStack(_stack,std::make_pair(row-1,col-1)); //  add change to stack
            }
        }
    }

    _field = initBoard;

}

void GameModel::incrStack(std::map<std::pair<int, int>, int> stack, std::pair<int, int> key)
{
    if (!stack[key])
        stack[key] = 1;
    else
    {
        if (stack[key] < 3)
            stack[key] += 1;
        else
            stack[key] = 0;
    }
}


TileModel& GameModel::getTile(std::pair<int, int> coord)
{
    return _field[coord.first][coord.second];
}

bool GameModel::solved()
{
    std::vector<std::pair<int, int>> btRoute; // route tracker
    btRoute.push_back(_startTile);

    //  remove all nodes from connected Tiles
    // ALTERNATIVE: for (auto coord = btRoute.begin(); coord != btRoute.end(); coord++) 
    for (std::pair<int, int> coord : btRoute)
    {
        if (getTile(coord).detached())
        {
            btRoute.erase(btRoute.begin());
            continue;
        }
            
        if (getTile(coord).north)
        {
            getTile(coord).north = false ;
            if (coord.second != 0)
            {
                btRoute.push_back(std::make_pair(coord.first, coord.second + 1));
                getTile(std::make_pair(coord.first, coord.second + 1)).south = false ;
            }
        }
        if (getTile(coord).south)
        {
            getTile(coord).south = false ;
            if (coord.second != _DIMY - 1)
            {
                btRoute.push_back(std::make_pair(coord.first, coord.second - 1));
                getTile(std::make_pair(coord.first, coord.second - 1)).north = false ;
            }
        }
        if (getTile(coord).east)
        {
            getTile(coord).east = false ;
            if (coord.first != _DIMX - 1)
            {
                btRoute.push_back(std::make_pair(coord.first + 1, coord.second));
                getTile(std::make_pair(coord.first + 1, coord.second)).west = false ;
            }
            
        }
        if (getTile(coord).west)
        {
            getTile(coord).west = false;
            if (coord.first != 0)
            {
                btRoute.push_back(std::make_pair(coord.first - 1, coord.second));
                getTile(std::make_pair(coord.first - 1, coord.second)).west = false ;
            }
        }

        btRoute.erase(btRoute.begin());
    }

    //  check if any attached nodes left
    for (size_t row = 1; row <= _DIMX; ++row){
        for (size_t col = 1; col <= _DIMY; ++col){
            if (!_field[row - 1][col - 1].detached())
                return false;
        }
    }
    return true;
}
