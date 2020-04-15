/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

 /*
 * \author Christian Bouchard, glm, jordan
 */

#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <math.h>
#include <exception>
#include "Cell.hpp"
#include <Eigen/Dense>

class Grid {
public:
    Grid(double xMin, double xMax, double yMin, double yMax, double cellSize,std::vector<Eigen::Vector3d *> & pointCloud);
    virtual ~Grid();

    void addPoint(Eigen::Vector3d * point);

    void computeCellFeatures();

    // Get/Set

    unsigned int getNbCellsAlongX()			{ return nbCellsAlongX;};
    unsigned int getNbCellsAlongY()			{ return nbCellsAlongY;};
    std::vector<std::vector<Cell> > & getCells()	{ return cells;};

private:
    double xMin;
    double xMax;

    double yMin;
    double yMax;

    double cellSize;

    unsigned int nbCellsAlongX;
    unsigned int nbCellsAlongY;

    std::vector< std::vector< Cell > > cells;
};

#endif /* GRID_HPP */

