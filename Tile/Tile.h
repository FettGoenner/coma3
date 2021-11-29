#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <QVector>

// Basisklasse: hält grundlegende Funktionen der Tiles
class Tile
{
/*** private Attribute ***/

    //          < west, ^ nord, v süd, > ost
    // oder     ost >, nord ^, west <, süd v
    QVector<bool> _nodes () ;

    // Rotationswinkel (wichtig für QPainter)
    size_t _rangle ;

public:
/*** Konstruktoren ***/
    // Standardkonstruktor
    Tile ()
    {}

/*** set-Methoden ***/

    // rotiere Tile um 90° im Uhrzeigersinn (?)
    void rotate ()
    {
        // shift nodes
    }

/*** get-Methoden ***/

    bool east ()
    {
        return _nodes[0] ;
    }

    bool north ()
    {
        return _nodes[1] ;
    }

    bool west ()
    {
        return _nodes[2] ;
    }

    bool south ()
    {
        return _nodes[3] ;
    }
}

#endif // TILE_H
