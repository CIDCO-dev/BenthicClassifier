/*
* Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

class Point
{

public:

    Point( double x, double y, double z )
    : x(x), y(y), z(z)
    {}

//    ~Point() {
//        std::cout << "Point::~Point()\n";
//    }
    
    
    double getX() { return x; }
    
    double getY() { return y; }
    
    double getZ() { return z; }
    
    

private:

    double x;
    double y;
    double z;
};

#endif
