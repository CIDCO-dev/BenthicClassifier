/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

 /*
 * \author Christian Bouchard, jordan
 */

#include "Grid.hpp"

Grid::Grid(double xMin, double xMax, double yMin, double yMax, double cellSide) : xMin( xMin ), xMax( xMax ),
        yMin( yMin ), yMax( yMax ),
        cellSide( std::abs(cellSide) ),
        nbCellsAlongX( static_cast< unsigned int >( ceil( ( xMax - xMin ) / std::abs(cellSide) ) ) ),
        nbCellsAlongY( static_cast< unsigned int >( ceil( ( yMax - yMin ) / std::abs(cellSide) ) ) ) {
    
    std::cout << "\nGrid::Grid()\n"
            << "nbCellsAlongX: " << nbCellsAlongX
            << "\nnbCellsAlongY: " << nbCellsAlongY
            << "\nnbCellsAlongX * nbCellsAlongY: " << nbCellsAlongX * nbCellsAlongY << "\n" << std::endl;

    cells.reserve(nbCellsAlongX);

    for (unsigned int count = 0; count < nbCellsAlongX; count++) {
        cells.push_back(std::vector< Cell > (nbCellsAlongY));
    }

    std::cout << "\nEnd of constructor\n" << std::endl;
}

Grid::~Grid() {
}

void Grid::addPoint(Eigen::Vector3d point) {
    double xPoint = point[0];
    double yPoint = point[1];

    if (xPoint < xMin || xPoint > xMax || yPoint < yMin || yPoint > yMax) {
        std::cerr << "\n\n----- Function Grid::addPoint(): point ( x, y ) = ( " << xPoint
                << ", " << yPoint << " )\n"
                << "this is not with the range that was provided to the constructor which was:\n"
                << "x: [ " << xMin << ", " << xMax << " ]\n"
                << "y: [ " << yMin << ", " << yMax << " ]\n"
                << "Point not put into the cell\n" << std::endl;
        return;
    }


    int xIndex = static_cast<int> (floor((xPoint - xMin) / cellSide));
    int yIndex = static_cast<int> (floor((yPoint - yMin) / cellSide));

    cells[ xIndex ] [ yIndex ].addPoint(point);
}

int Grid::getNbOfPointsForCell(const unsigned int indexX, const unsigned int indexY, bool & OK) {
    OK = false;

    if (indexX >= 0 && indexX < nbCellsAlongX
            && indexY >= 0 && indexY < nbCellsAlongY) {
        OK = true;
        return cells[ indexX ][ indexY ].getNbOfPoints();
    } else {
        return 0;
    }
}

bool Grid::computeFeaturesForCell(const unsigned int indexX, const unsigned int indexY) {

    bool OK = false;

    if (indexX >= 0 && indexX < nbCellsAlongX
            && indexY >= 0 && indexY < nbCellsAlongY) {
        OK = cells[ indexX ][ indexY ].computeFeatures();
    }

    return OK;
}

void Grid::getFeaturesForCell(const unsigned int indexX, const unsigned int indexY,
        std::vector< double > & vectorOut,
        bool & OK) {
    OK = false;

    if (indexX >= 0 && indexX < nbCellsAlongX
            && indexY >= 0 && indexY < nbCellsAlongY) {
        OK = true;
        vectorOut = cells[ indexX ][ indexY ].getFeatures();
    }
}

void Grid::display(bool displayEmptyCell /*= false*/) {

    std::cout << "\nGrid, nbCellsAlongX: " << nbCellsAlongX
            << "\nnbCellsAlongY: " << nbCellsAlongY
            << "\nnbCellsAlongX * nbCellsAlongY: " << nbCellsAlongX * nbCellsAlongY << "\n\n";

    for (int unsigned countX = 0; countX < nbCellsAlongX; countX++) {

        for (int unsigned countY = 0; countY < nbCellsAlongY; countY++) {

            if (cells[ countX ][ countY ].getNbOfPoints() > 0
                    || displayEmptyCell) {

                std::cout << "Cell index " << countX << ", " << countY
                        << ", boudary, x: "
                        << countX * cellSide + xMin << " to "
                        << (countX + 1) * cellSide + xMin
                        << ", y: "
                        << countY * cellSide + yMin << " to "
                        << (countY + 1) * cellSide + yMin << "\n";

                cells[ countX ][ countY ].display();

                std::cout << "\n";
            }

        }

    }

}

unsigned int Grid::getNbCellsAlongX() {
    return nbCellsAlongX;
}

unsigned int Grid::getNbCellsAlongY() {
    return nbCellsAlongY;
}

std::vector<std::vector<Cell> > & Grid::getCells() {
    return cells;
}

