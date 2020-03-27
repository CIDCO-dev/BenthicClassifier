#ifndef TESTGRID_HPP
#define TESTGRID_HPP

#include "catch.hpp"

#include "../src/Point.hpp"
#include "../src/Grid.hpp"

TEST_CASE("Test Grid")
{

    Point point1( -64.721366, 1.033044, 14.634260 );
    Point point2( -64.690600, 1.188072, 14.637367 );
    Point point3( -64.659785, 1.343379, 14.640452 );
    Point point4( -64.628484, 1.501377, 14.642026 );
    Point point5( -64.597135, 1.659657, 14.643562 );
    Point point6( -64.567047, 1.810989, 14.649506 );
    Point point7( -64.536469, 1.965035, 14.653945 );
    Point point8( -64.506705, 2.114590, 14.661365 );
    Point point9( -64.475585, 2.271657, 14.664253 );
    Point point10( -64.444844, 2.426642, 14.668615 );


    double xMin = -64.721366;
    double xMax = -64.444844;

    double yMin = 1.033044;
    double yMax = 2.426642;

    double cellSide = 0.2;

    Grid grid ( xMin, xMax, yMin, yMax, cellSide );

    const int expectedNbCellsAlongX = 2;
    const int expectedNbCellsAlongY = 7;

    REQUIRE( grid.getNbCellsAlongX() == expectedNbCellsAlongX );
    REQUIRE( grid.getNbCellsAlongY() == expectedNbCellsAlongY );


    bool OK;
    int number;

    // Before putting points in the grid, the number of points
    // should be zero for all cells
    for ( int indexX = 0; indexX < grid.getNbCellsAlongX(); indexX++ ) {
        for ( int indexY = 0; indexY < grid.getNbCellsAlongY(); indexY++ ) {
            number = grid.getNbOfPointsForCell( indexX, indexY, OK );
            REQUIRE( OK == true );
            REQUIRE( number == 0 );
        }
    }


    grid.addPoint( &point1 );
    grid.addPoint( &point2 );
    grid.addPoint( &point3 );
    grid.addPoint( &point4 );
    grid.addPoint( &point5 );
    grid.addPoint( &point6 );
    grid.addPoint( &point7 );
    grid.addPoint( &point8 );
    grid.addPoint( &point9 );
    grid.addPoint( &point10 );


    int expectedNbOfPointsForCells[ expectedNbCellsAlongX ][ expectedNbCellsAlongY ]
            = { { 2, 1, 1, 2, 1, 0, 0 },
                { 0, 0, 0, 0, 0, 1, 2 } };

    for ( int indexX = 0; indexX < grid.getNbCellsAlongX(); indexX++ ) {
        for ( int indexY = 0; indexY < grid.getNbCellsAlongY(); indexY++ ) {
            number = grid.getNbOfPointsForCell( indexX, indexY, OK );
            REQUIRE( OK == true );
            REQUIRE( number == expectedNbOfPointsForCells[ indexX ][ indexY ] );
        }
    }

}

#endif
