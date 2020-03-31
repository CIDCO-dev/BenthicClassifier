/*
* Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */


#ifndef GRID_HPP
#define GRID_HPP


#include <vector>
#include <math.h>

#include "Cell.hpp"

class Grid
{

public:

    Grid( double xMin, double xMax, double yMin, double yMax, double cellSide )
    :   xMin( xMin ), xMax( xMax ),
        yMin( yMin ), yMax( yMax ),
        cellSide( cellSide ),
        nbCellsAlongX( static_cast< int >( ceil( ( xMax - xMin ) / cellSide ) ) ),
        nbCellsAlongY( static_cast< int >( ceil( ( yMax - yMin ) / cellSide ) ) )
    {

        std::cout << "\nGrid::Grid()\n" 
            <<   "nbCellsAlongX: " << nbCellsAlongX
            << "\nnbCellsAlongY: " << nbCellsAlongY 
            << "\nnbCellsAlongX * nbCellsAlongY: " << nbCellsAlongX * nbCellsAlongY << "\n" << std::endl;

        cells.reserve( nbCellsAlongX );

        for ( int count = 0; count < nbCellsAlongX; count++ ) {
            cells.push_back( std::vector< Cell > ( nbCellsAlongY ) );
        }

        std::cout << "\nEnd of constructor\n" << std::endl;    
    }


//    ~Grid() {
//    }


    void addPoint( Point * point )
    {
        double xPoint = point->getX();
        double yPoint = point->getY();

        if ( xPoint < xMin || xPoint > xMax || yPoint < yMin || yPoint > yMax ){
            std::cout << "\n\n----- Function Grid::addPoint(): point ( x, y ) = ( " << xPoint
                    << ", " << yPoint << " )\n"
                    << "this is not with the range that was provided to the constructor which was:\n"
                    << "x: [ " << xMin << ", " << xMax << " ]\n"
                    << "y: [ " << yMin << ", " << yMax << " ]\n"
                    << "Point not put into the cell\n" << std::endl;
            return;
        }


        int xIndex = static_cast< int >( floor( ( xPoint - xMin ) / cellSide ) );
        int yIndex = static_cast< int >( floor( ( yPoint - yMin ) / cellSide ) );

        // std::cout << "\n\n----- Function Grid::addPoint(): point ( x, y ) = ( " << xPoint
        //             << ", " << yPoint << " )\n"
        //             << "( xIndex, yIndex ): ( " << xIndex
        //             << ", " << yIndex << " )\n" << std::endl;

        cells[ xIndex ] [ yIndex ].addPoint( point );
    }


    int getNbOfPointsForCell( const int indexX, const int indexY, bool & OK )
    {
        OK = false;

        if ( indexX >= 0 && indexX < nbCellsAlongX
                && indexY >= 0 && indexY < nbCellsAlongY ) {
            OK = true;
            return cells[ indexX ][ indexY ].getNbOfPoints();
        } else {
            return 0;
        }
    }


    bool computeFeaturesForCell( const int indexX, const int indexY ) {

        bool OK = false;

        if ( indexX >= 0 && indexX < nbCellsAlongX
                && indexY >= 0 && indexY < nbCellsAlongY ) {
            OK = cells[ indexX ][ indexY ].computeFeatures();
        }

        return OK;
    }


    void getFeaturesForCell( const int indexX, const int indexY,
                                std::vector< double > & vectorOut,
                                bool & OK ) {
        OK = false;

        if ( indexX >= 0 && indexX < nbCellsAlongX
                && indexY >= 0 && indexY < nbCellsAlongY ) {
            OK = true;
            vectorOut = cells[ indexX ][ indexY ].getFeatures();
        }
    }


    void display( bool displayEmptyCell = false ) {

        std::cout << "\nGrid, nbCellsAlongX: " << nbCellsAlongX
            << "\nnbCellsAlongY: " << nbCellsAlongY
            << "\nnbCellsAlongX * nbCellsAlongY: " << nbCellsAlongX * nbCellsAlongY << "\n\n";

        for ( int countX = 0; countX < nbCellsAlongX; countX++ ) {

            for ( int countY = 0; countY < nbCellsAlongY; countY++ ) {

                if ( cells[ countX ][ countY ].getNbOfPoints() > 0
                     || displayEmptyCell ) {

                    std::cout << "Cell index " << countX << ", " << countY
                              << ", boudary, x: "
                                  << countX * cellSide + xMin << " to "
                                  << ( countX +1 ) * cellSide + xMin
                              << ", y: "
                                  << countY * cellSide + yMin << " to "
                                  << ( countY +1 ) * cellSide + yMin << "\n";

                    cells[ countX ][ countY ].display();

                    std::cout << "\n";
                }

            }

        }

    }





    int getNbCellsAlongX()
    { 
        return nbCellsAlongX;
    }

    int getNbCellsAlongY()
    { 
        return nbCellsAlongY;
    }    



private:

// Variables

    // min/max x, y of points
    double xMin;
    double xMax;

    double yMin;
    double yMax;

    double cellSide;

    int nbCellsAlongX;
    int nbCellsAlongY;

    std::vector< std::vector< Cell > > cells;

};

#endif
