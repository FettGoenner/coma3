#ifndef TILEMODEL_H
#define TILEMODEL_H

#include <QObject>

// Die TileModel-Klasse repräsentiert die Funktionalität
// der drehbaren Quadrate im Netzwerkpuzzle ohne Visualisierung.
class TileModel
    : public QObject
{
    Q_OBJECT

public:
/*** public attributes ***/

    // nodes
    bool north ;
    bool south ;
    bool east ;
    bool west ;

/*** constructors ***/

    // randomized generation
    TileModel()
        : north(false),
          south(false),
          east (false),
          west (false)
    { }

    TileModel(bool north, bool south, bool east, bool west) 
        : north(north),
          south(south),
          east (east),
          west (west)
    { }

    ~TileModel()
    { }

/*** setters ***/
    void rotate()
    {
        bool temp = north ;
        north = west ;
        west  = south ;
        south = east ;
        east  = temp ;

        emit(changed()) ;
    }

/*** getters ***/

    // amount of active nodes
    size_t nNodes() const
    {
        size_t n = 0 ;
        if (north)
            ++n ;
        if (south)
            ++n ;
        if (east)
            ++n ;
        if (west)
            ++n ;
        return n ;
    }

signals:
    void changed() ;
} ;

#endif // TILEMODEL_H
