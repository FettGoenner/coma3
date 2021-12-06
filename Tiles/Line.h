#pragma once
#include "Tile.h"

#include <string>

class Line :
    public Tile
{
public:

    //  CONSTRUCTORS
    Line();
    Line(Tile const&);

    //  DESTRUCTOR
    ~Line();

    // ASSIGNING OPERATOR
    void operator = (const Tile& tile);

    //  return Tile type
    std::string type() const;
};

