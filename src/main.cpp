/*
* Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

/*
 * \author Christian Bouchard, Guillaume Morissette
 */

#ifndef _WIN32
    #include <getopt.h>
#endif


#include <iostream>
#include <iomanip>
#include <string>

#include <sstream>

#include <vector>

#include <limits>

#include "geometry/Grid.hpp"

#include "machinelearning/DBSCAN.hpp"

/**
* Main
*
* @param argc number of CLI arguments
* @param argv value of the CLI arguments
*/
int main( int argc, char* argv[] )
{
    double cellSize = 1.0;

    // Get cell size parameter from command line, default to 1.0 if not present
    int index;
    while((index=getopt(argc,argv,"s:"))!=-1)
    {
        switch(index)
        {
            case 's':

                if (sscanf(optarg,"%lf", &cellSize) != 1)
                {
                    std::cerr << "Invalid cell size value (-s)" << std::endl;
                    exit(1);
                }

                if ( cellSize <= 0 )
                {
                    std::cerr << "Cell size must be greater than zero (-s)" << std::endl;
                    exit(1);
                }

                break;
        }
    }

    std::cerr << "Using cell size of " << cellSize << "m" << std::endl;

    // Read point cloud from stdin using georeference output format
    // x y z quality intensity
    std::vector < Eigen::Vector3d * > allPoints;

    double xMin = std::numeric_limits<double>::max();
    double xMax = std::numeric_limits<double>::min();

    double yMin = std::numeric_limits<double>::max();
    double yMax = std::numeric_limits<double>::min();

    double valueX;
    double valueY;
    double valueZ;

    std::string line;

    while ( std::getline( std::cin, line ) ) {

        std::istringstream stream( line );

        if ( stream >> valueX >> valueY >> valueZ ) {
	    try{
	            allPoints.push_back( new Eigen::Vector3d( valueX, valueY, valueZ ) );

        	    if ( valueX < xMin ){
                	xMin = valueX;
	            }
            	    if ( valueX > xMax ){
                	xMax = valueX;
	   	    }
	            if ( valueY < yMin ){
                	yMin = valueY;
	            }
	            if ( valueY > yMax ){
        	        yMax = valueY;
	            }
             }
	     catch(std::exception & e){
		std::cerr <<  e.what() << std::endl;
	     }
        }
    }

    std::cerr << allPoints.size() << " points read" << std::endl; 

    //Create grid and compute features for all cells
    Grid grid ( xMin, xMax, yMin, yMax, cellSize,allPoints);

    std::cerr << "Using " << grid.getNbCellsAlongX() << "x" << grid.getNbCellsAlongY() << " grid (" << grid.getNbCellsAlongX()*grid.getNbCellsAlongY() << " cells)" << std::endl;

    grid.computeCellFeatures();

    //flatten Cell vector and cluster
    std::vector<Cell*> flattenedCells;

    for(auto i=grid.getCells().begin();i!=grid.getCells().end();i++){
        for(auto j=(*i).begin();j!=(*i).end();j++){
		if((*j).areFeaturesComputed()){
			flattenedCells.push_back(&(*j));
		}
        }
    }

/*
    for(auto i=grid.getCells().begin();i!=grid.getCells().end();i++){
	for(auto j=(*i).begin();j!=(*i).end();j++){
		if((*j)->areFeaturesComputed()){
			std::cout << (*j)->getFeatureVector().transpose() << std::endl;
		}
	}
    }
*/

    std::cerr << flattenedCells.size() << " cells to cluster" << std::endl;

    DBSCAN clustering(0.1,2,flattenedCells);

    clustering.run();

    for (unsigned int classDBSCAN = 0; classDBSCAN < clustering.getCluster().size(); classDBSCAN++) {
        for (unsigned int cellIdx = 0; cellIdx < clustering.getCluster()[classDBSCAN].size(); cellIdx++) {
            Cell cell = * flattenedCells[clustering.getCluster()[classDBSCAN][cellIdx]];
            std::vector< double > features = cell.getFeatures();

            for (unsigned int p = 0; p < cell.getPoints().size(); p++) {
                Eigen::Vector3d * point = cell.getPoints()[p];

                std::cout << (*point)[0] << " "
                        << (*point)[1] << " "
                        << (*point)[2] << " "
                        << classDBSCAN << " ";

                for(unsigned int featureIndex = 0; featureIndex < features.size(); featureIndex++) {
                    std::cout << features[featureIndex];
                    if(featureIndex != features.size()-1) {
                        std::cout << " ";
                    }
                }

                std::cout << std::endl;
            }
        }
    }
}
