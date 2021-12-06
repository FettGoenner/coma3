#include "TileModelModel.h"

TileModel::TileModel()
    : north(false),
      south(false),
      east (false),
      west (false)
{ }

TileModel::TileModel(bool north, bool south, bool east, bool west) 
    : north(north),
      south(south),
      east (east),
      west (west)
{ }

void TileModel::rotate()
{
    bool temp = north ;
    north = west ;
    west  = south ;
    south = east ;
    east  = temp ;
}

bool TileModel::detached() const
{
    if (north || south || east || west)
        return false;
    return true ;
}

/* bool TileModel::north() */
/* { */
/*     return this->_north ; */
/* } */
/* bool TileModel::south() */
/* { */
/*     return this->_south ; */
/* } */
/* bool TileModel::east() */
/* { */
/*     return this->_east ; */
/* } */
/* bool TileModel::west() */
/* { */
/*     return this->_west ; */
/* } */

/* void TileModel::setNorth(bool val) */ 
/* { */
/*     _north = val; */
/* } */
/* void TileModel::setSouth(bool val) */
/* { */
/*     _south = val; */
/* } */
/* void TileModel::setEast(bool val) */
/* { */
/*     _east = val; */
/* } */
/* void TileModel::setWest(bool val) */
/* { */
/*     _west = val; */
/* } */
