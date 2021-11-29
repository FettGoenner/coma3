#ifndef END_H
#define END_H

#include "Tile.h"
#include <string>

// End-Klasse (von Tile abgeleitet)
class End
    : public Tile
{

public:

/*** Konstruktoren ***/

    // Standardkonstruktor
    End ()
        : Tile( "end", 0 )
    { }

    // Initialisierungskonstruktor
    // konstruiert mit n Rotationen
    End ( size_t n )
        : Tile( "end", n )
    { }

} ;

#endif // END_H
