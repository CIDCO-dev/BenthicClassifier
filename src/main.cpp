/*
* Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef _WIN32
    #include <getopt.h>
#endif


#include <iostream>
#include <iomanip>
#include <string>

#include <sstream>

#include <vector>

#include <memory>

#include <limits>       // std::numeric_limits

#include "Point.hpp"
#include "Grid.hpp"


/**
* Declares the program depending on argument received
*
* @param argc number of argument
* @param argv value of the arguments
*/
int main( int argc, char* argv[] )
{


    std::cout << "\nBenthicClassifier, using input on std::cin\n"
        << "CTRL-d to end if no EOF\n" << std::endl;

    double cellSide = 1.0;


    int index;
    while((index=getopt(argc,argv,"s:"))!=-1)
    {
        switch(index)
        {
            case 's':

                if (sscanf(optarg,"%lf", &cellSide) != 1)
                {
                    std::cerr << "Invalid cell side width value (-s)" << std::endl;
                    exit(1);
                }

                if ( cellSide <= 0 )
                {
                    std::cerr << "Invalid cell side width (-s), cell side width: " << cellSide
                        << ",\nit must be above zero." << std::endl;
                    exit(1);
                }

                break;

        }
    }


    std::cout << "cellSide: " << cellSide << "\n" << std::endl;




    // LGF: NED
    // From MBES-lib's DatagramGeoreferencer::processGeoreferencedPing()
    // std::cout << georeferencedPing(0) << " " << georeferencedPing(1) << " " << georeferencedPing(2) << " " << quality << " " << intensity << std::endl;


    std::vector < std::shared_ptr< Point > > allPoints;


    double xMin = std::numeric_limits<double>::max();
    double xMax = - std::numeric_limits<double>::max();
    
    double yMin = std::numeric_limits<double>::max();
    double yMax = - std::numeric_limits<double>::max();
    

    std::cout << "\nxMin: " << xMin
        << "\nxMax: " << xMax
        << "\nyMin: " << yMin
        << "\nyMax: " << yMax << std::endl;

    
    double valueX;
    double valueY;
    double valueZ;    
    

    std::string entireLine;

    while ( std::getline( std::cin, entireLine ) ) {

        std::istringstream stream( entireLine );

        if ( stream >> valueX >> valueY >> valueZ ) {
            
            std::shared_ptr< Point > point = std::make_shared< Point > ( valueX, valueY, valueZ );

            allPoints.push_back( point );
                        
            if ( valueX < xMin )
                xMin = valueX;
                
            if ( valueX > xMax )
                xMax = valueX;
                                
            if ( valueY < yMin )
                yMin = valueY;
                
            if ( valueY > yMax )
                yMax = valueY;
        }

    }
                        
                        
    std::cout << "\nDone reading from std::cin\n" << std::endl; 
    
    
    std::cout << std::fixed << std::setprecision( 8 );    
    
    
    std::cout << "\nxMin: " << xMin
        << "\nxMax: " << xMax
        << "\nyMin: " << yMin
        << "\nyMax: " << yMax << "\n" << std::endl;
    
    
    // Display the list of Points
    for ( unsigned int count = 0; count < allPoints.size(); count++ )
        std::cout << "BenthicClassifier, " 
        << allPoints[ count ]->getX() << " "
        << allPoints[ count ]->getY() << " "
        << allPoints[ count ]->getZ() << std::endl;




    Grid grid ( xMin, xMax, yMin, yMax, cellSide );

    for ( unsigned int count = 0; count < allPoints.size(); count++ )
        grid.addPoint( allPoints[ count ] );


    grid.display();

        
    std::cout << "\nEnd of BenthicClassifier \n" << std::endl;    
        
}
