#include "End.h"

End::End()
    :Tile(0, 1, 0, 0)
{}

End::End(Tile const& tile)
    : Tile(tile)
{
    this->_south = 1;
}


End::~End() {};

void End::operator = (const Tile& tile)
{
    *this = End(tile);
    this->_south = 1;
}

std::string End::type() const
{
    return "End";
}