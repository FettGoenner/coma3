#ifndef LINE_H
#define LINE_H

#include "Tile.h"
#include <string>

// Line-Klasse (von Tile abgeleitet)
class Line
    : public Tile
{

public:

/*** Konstruktoren ***/

    // Standardkonstruktor
    Line ()
        : Tile( "line", 0 )
    { }

    // Initialisierungskonstruktor
    // konstruiert mit n Rotationen
    Line ( size_t n )
        : Tile( "line" , n)
    { }

} ;

#endif // LINE_H
