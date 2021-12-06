#pragma once
#include <QStack>
#include <QRandomGenerator>
#include <stack>
#include <vector>
#include <utility>
#include <map>

#include "TileModel.h"

class GameModel
    : public QObject
{
    Q_OBJECT

    // GameModel dimensions
    size_t  _DIMX; 
    size_t  _DIMY;
    std::pair<int, int> _startTile;

    TileModel** _field;      //  TileModel field
    
    std::map<std::pair<int,int>, int> _stack; //  Stack for edit history

public:

    // CONSTRUCTORS
    GameModel();
    GameModel(TileModel**, int, int, size_t, size_t);

    //  minimal stack incrementation
    void incrStack(std::map<std::pair<int, int>, int>, std::pair<int, int>);

    //  SOLUTION CHECKER
    bool solved();

    //  Returns corresponding TileModel
    TileModel& getTile(std::pair<int,int>);
};

