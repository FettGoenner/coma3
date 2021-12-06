#pragma once
#include "Tile.h"

#include <string>

class Junction :
    public Tile
{
public:

    //  CONSTRUCTORS
    Junction();
    Junction(Tile const&);

    //  DESTRUCTOR
    ~Junction();

    // ASSIGNING OPERATOR
    void operator = (const Tile& tile);

    //  return Tile type
    std::string type() const;
};

