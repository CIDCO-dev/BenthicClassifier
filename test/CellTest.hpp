#ifndef TESTCELL_HPP
#define TESTCELL_HPP

#include "catch.hpp"


#include "../src/Cell.hpp"
#include "../src/Point.hpp"


TEST_CASE("Test the constructor, adding points, clearing cell")
{

    Cell cell;

    REQUIRE( cell.getNbOfPoints() == 0 );

    Point point1( -64.721366, 1.033044, 14.634260 );
    Point point2( -64.690600, 1.188072, 14.637367 );
    Point point3( -64.659785, 1.343379, 14.640452 );

    cell.addPoint( &point1 );
    cell.addPoint( &point2 );
    cell.addPoint( &point3 );

    REQUIRE( cell.getNbOfPoints() == 3 );

    cell.clear();

    REQUIRE( cell.getNbOfPoints() == 0 );

}

#endif
