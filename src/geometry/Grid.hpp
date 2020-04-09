/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

 /*
 * \author Christian Bouchard, jordan
 */

#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <math.h>
#include "Cell.hpp"
#include <Eigen/Dense>

class Grid {
public:
    Grid(double xMin, double xMax, double yMin, double yMax, double cellSide);
    virtual ~Grid();

    void addPoint(Eigen::Vector3d point);
    int getNbOfPointsForCell(const unsigned int indexX, const unsigned int indexY, bool & OK);
    bool computeFeaturesForCell(const unsigned int indexX, const unsigned int indexY);
    void getFeaturesForCell(
            const unsigned int indexX,
            const unsigned int indexY,
            std::vector< double > & vectorOut,
            bool & OK);
    
    void display( bool displayEmptyCell = false );    
    unsigned int getNbCellsAlongX();
    unsigned int getNbCellsAlongY();    
    std::vector<std::vector<Cell> > & getCells();

private:
    
    double xMin;
    double xMax;

    double yMin;
    double yMax;

    double cellSide;

    unsigned int nbCellsAlongX;
    unsigned int nbCellsAlongY;

    std::vector< std::vector< Cell > > cells;

};

#endif /* GRID_HPP */

