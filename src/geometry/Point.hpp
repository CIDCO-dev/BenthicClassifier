/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

 /*
 * \author Christian Bouchard, jordan
 */

#ifndef POINT_HPP
#define POINT_HPP

class Point {
public:
    Point(double x, double y, double z);
    virtual ~Point();
    
    double getX();
    double getY();
    double getZ();
    
private:
    
    double x;
    double y;
    double z;

};

#endif /* POINT_HPP */

