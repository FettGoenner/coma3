#pragma once
#include "Tile.h"

#include <string>

class Corner :
    public Tile
{
public:

    //  CONSTRUCTORS
    Corner();
    Corner(Tile const&);

    //  DESTRUCTOR
    ~Corner();

    // ASSIGNING OPERATOR
    void operator = (const Tile& tile);

    //  return Tile type
    std::string type() const;

    
};
