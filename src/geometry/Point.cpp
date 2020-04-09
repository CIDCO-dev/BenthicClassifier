/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

 /*
 * \author Christian Bouchard, jordan
 */

#include "Point.hpp"

Point::Point(double x, double y, double z)
: x(x), y(y), z(z) {
}

Point::~Point() {
}

double Point::getX() {
    return x;
}

double Point::getY() {
    return y;
}

double Point::getZ() {
    return z;
}



