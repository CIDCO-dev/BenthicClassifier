/*
* Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef CELL_HPP
#define CELL_HPP


#include <iostream>
#include <vector>
#include <memory>

#include "Point.hpp"

class Cell
{

public:

    Cell() {}

    ~Cell() {}

    void addPoint( std::shared_ptr< Point > point ) {
        points.push_back( point );
    }

    int getNbOfPoints() {
        return points.size();
    }

    void clear() {
        points.clear();
    }

    void display() {
        std::cout << "Cell: " << getNbOfPoints() << " point(s)\n";

        for ( unsigned int count = 0; count < points.size(); count++ ) {

            std::cout << points[ count ]->getX() << " "
                        << points[ count ]->getY() << " "
                        << points[ count ]->getZ() << "\n";
        }

        std::cout << std::endl;
    }


private:


// Variables

    std::vector< std::shared_ptr< Point > > points;

};



#endif
