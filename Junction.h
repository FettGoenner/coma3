#ifndef JUNCTION_H
#define JUNCTION_H

#include "Tile.h"
#include <string>

// Junction-Klasse (von Tile abgeleitet)
class Junction
    : public Tile
{

public:

/*** Konstruktoren ***/

    // Standardkonstruktor
    Junction ()
        : Tile( "junction", 0 )
    { }

    // Initialisierungskonstruktor
    // konstruiert mit n Rotationen
    Junction ( size_t n)
        : Tile( "junction", n )
    { }

} ;

#endif // JUNCTION_H
