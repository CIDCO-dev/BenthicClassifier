/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

/*
 * \author Christian Bouchard, jordan
 */

#include "Grid.hpp"

Grid::Grid(double xMin, double xMax, double yMin, double yMax, double cellSize,std::vector<Eigen::Vector3d *> & pointCloud) : xMin( xMin ), xMax( xMax ),
        yMin( yMin ), yMax( yMax ),
        cellSize( std::abs(cellSize) ),
        nbCellsAlongX( static_cast< unsigned int >( ceil( ( xMax - xMin ) / std::abs(cellSize) ) ) ),
        nbCellsAlongY( static_cast< unsigned int >( ceil( ( yMax - yMin ) / std::abs(cellSize) ) ) ) {

    cells.reserve(nbCellsAlongX);

    //std::cout << nbCellsAlongX << " by "<< nbCellsAlongY << std::endl;

    for (unsigned int countX = 0; countX < nbCellsAlongX; countX++) {
        cells.push_back(std::vector< Cell > (nbCellsAlongY));
    }

    for(auto i=pointCloud.begin();i!=pointCloud.end();i++){
	addPoint(*i);
    }
}

Grid::~Grid() {

}

void Grid::addPoint(Eigen::Vector3d * point) {

    if ( (*point)[0] < xMin || (*point)[0] > xMax || (*point)[1] < yMin || (*point)[1] > yMax) {
	throw std::logic_error("point is outside of grid");
    }

    int xIndex = static_cast<int> (floor(((*point)[0] - xMin) / cellSize));
    int yIndex = static_cast<int> (floor(((*point)[1] - yMin) / cellSize));

    //std::cout << xIndex << " " << yIndex << std::endl;

    cells[xIndex][yIndex].addPoint(point);
}

void Grid::computeCellFeatures(){
    for(auto i=cells.begin();i!=cells.end();i++){
	for(auto j=(*i).begin();j!=(*i).end();j++){
		if( !(*j).computeFeatures() ){
			//TODO: log? we already whine in the Cell class
			//std::cerr << "Skipped cell" << std::endl; //TODO: give indexes
		}
	}
    }
}
