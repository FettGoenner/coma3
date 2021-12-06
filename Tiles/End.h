#pragma once
#include "Tile.h"

#include <string>

class End :
    public Tile
{
public:

    //  CONSTRUCTORS
    End();
    End(Tile const&);

    //  DESTRUCTOR
    ~End();

    // ASSIGNING OPERATOR
    void operator = (const Tile& tile);

    //  return Tile type
    std::string type() const;
};

