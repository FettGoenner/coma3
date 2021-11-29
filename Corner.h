#ifndef CORNER_H
#define CORNER_H

#include "Tile.h"
#include <string>

// Corner-Klasse (von Tile abgeleitet)
class Corner
    : public Tile
{

public:

/*** Konstruktoren ***/

    // Standardkonstruktor
    Corner ()
        : Tile( "corner", 0 )
    { }

    // Initialisierungskonstruktor
    // konstruiert mit n Rotationen
    Corner ( size_t n)
        : Tile( "corner", n )
    { }

} ;

#endif // CORNER_H
