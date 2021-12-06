#ifndef TILEMODEL_H
#define TILEMODEL_H

#include <QObject>
#include <string>

using namespace std ;

class TileModel
    : public QObject
{
protected:
public:

    /*** public attributes ***/
    bool north ;
    bool south ;
    bool east ;
    bool west ;

    string _type ; 

    size_t _rangle ;

    /*** constructors ***/
    TileModel() ;
    TileModel(bool, bool, bool, bool) ;

    /*** setters ***/
    void rotate() ;

    /*** getters ***/
    bool detached() const ;

    /*** getters ***/

    /* bool north(); */
    /* bool south(); */
    /* bool east(); */
    /* bool west(); */

    /*** setters ***/

    /* void setNorth(bool); */
    /* void setSouth(bool); */
    /* void setEast(bool); */
    /* void setWest(bool); */
};

#endif // TILEMODEL_H
