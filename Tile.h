#ifndef TILE_H
#define TILE_H

#include <string>
#include <iostream>
using namespace std ;

// Basisklasse: hält grundlegende Funktionen der Tiles
// abgeleitet werden: line, corner, junction, end
class Tile
{
/*** private Attribute ***/

    // Anschlüsse des Tiles an den vier Himmelsrichtungen
    bool _north = false ;
    bool _south = false ;
    bool _east  = false ;
    bool _west  = false ;

    // Typ des Tiles ("line", "corner", "junction" oder "end")
    string _type ;

    // Rotationswinkel (wichtig für QPainter)
    // ist 0,90,180 oder 270
    size_t _rangle = 0 ;

public:
/*** Konstruktoren ***/

    // Initialisierungskonstruktor
    // erzeugt bestimmten Typ von Tile in seiner "Standardposition"
    // und rotiert dann n-mal im Uhrzeigersinn.
    Tile ( string type, size_t n )
        : _type( type )
    {
        if ( type == "end" )
            _south = true ;
        else if ( type == "line" )
        {
            _east  = true ;
            _west  = true ;
        }
        else if ( type == "corner" )
        {
            _east  = true ;
            _south = true ;
        }
        else
        {
            // junction
            _east  = true ;
            _west  = true ;
            _south = true ;
        }
        rotate( n ) ;
    }

    // Initialisierungskonstruktor
    // erzeugt Tile durch Zuweisung der Himmelsrichtungen
    // ("line", "corner", "junction" oder "end")
    Tile ( bool north, bool south, bool east, bool west, string type )
        : _north(north),
          _south(south),
          _east(east),
          _west(west),
          _type(type)
    { }

/*** set-Methoden ***/

    // rotiere Tile um 90° im Uhrzeigersinn
    void rotate ()
    {
        bool temp = _north ;
        _north = _west  ;
        _west  = _south ;
        _south = _east  ;
        _east  = temp ;
        // passe Rotationswinkel an
        _rangle = ( _rangle + 90 ) % 360 ;
    }

    // rotiere Tile n-mal im Uhrzeigersinn
    void rotate ( size_t n )
    {
        for ( size_t i = 0 ; i < n ; ++i )
            rotate() ;
    }

/*** get-Methoden ***/

    // Gib Typ des Tiles zurück:
    string type () const
    {
        return _type ;
    }

    // Gibt Rotationswinkel des Tiles zurück
    size_t rangle () const
    {
        return _rangle ;
    }

    // Anschluss bei Himmelsrichtung?
    bool east () const
    {
        return _east ;
    }
    bool north () const
    {
        return _north ;
    }
    bool west () const
    {
        return _west ;
    }
    bool south () const
    {
        return _south ;
    }

/*** Ausgabeoperator ***/
    friend
    ostream& operator << ( ostream& ostr, Tile const t )
    {
        ostr << '*'      << t.north() << '*'      << endl ;
        ostr << t.west() << '*'       << t.east() << endl ;
        ostr << '*'      << t.south() << '*'              ;
        return ostr ;
    }
} ;

#endif // TILE_H
