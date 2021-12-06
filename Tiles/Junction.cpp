#include "Junction.h"

Junction::Junction()
    :Tile(0, 1, 1, 1)
{}

Junction::Junction(Tile const& tile)
    : Tile(tile)
{
    this->_south    = 1;
    this->_east     = 1;
    this->_west     = 1;
}


Junction::~Junction() {};

void Junction::operator = (const Tile& tile)
{
    *this = Junction(tile);
    this->_south    = 1;
    this->_east     = 1;
    this->_west     = 1;
}

std::string Junction::type() const
{
    return "Junction";
}
