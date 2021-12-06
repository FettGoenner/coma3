#include "Corner.h"
Corner::Corner()
    :Tile(0, 1, 1, 0)
{}

Corner::Corner(Tile const& tile)
    : Tile(tile)
{
    this->_south    = 1;
    this->_east     = 1;
}


Corner::~Corner() {};

void Corner::operator = (const Tile& tile)
{
    *this = Corner(tile);
    this->_south    = 1;
    this->_east     = 1;
}

std::string Corner::type() const
{
    return "Corner";
}