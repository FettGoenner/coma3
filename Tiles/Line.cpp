#include "Line.h"

Line::Line()
    :Tile(0,0,1,1)
{}

Line::Line(Tile const& tile)
    :Tile(tile)
{
    this->_east = 1;
    this->_west = 1;
}


Line::~Line() {};

void Line::operator = (const Tile& tile)
{
    *this = Line(tile);
    this->_east = 1;
    this->_west = 1;
}

std::string Line::type() const 
{
    return "Line";
}
