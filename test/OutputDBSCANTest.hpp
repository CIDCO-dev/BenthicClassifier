/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

#ifndef OUTPUTDBSCANTEST_HPP
#define OUTPUTDBSCANTEST_HPP

#include "catch.hpp"
#include "../src/geometry/Cell.hpp"
#include "../src/geometry/Grid.hpp"
#include "../src/machinelearning/DBSCAN.hpp"
#include "../test/utils/TestUtils.hpp"
#include "../src/io/ClassificationOutput.hpp"

TEST_CASE("Test output of classification") {
    
    /*
     * Create test periodical surface  element
     * 
     * The classification should be periodic since it is based on geometric
     * parameters.
     */
    double xmin = 2.0;
    double xmax = 10.0;

    double ymin = 3.0;
    double ymax = 11.0;
    
    int kx = 5;
    int ky = 10;
    
    double radius = std::sqrt(xmax*xmax + ymax*ymax);


    //number of points
    unsigned int n = 500;

    std::vector<double> X(n, 0.0);
    std::vector<double> Y(n, 0.0);

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distX(xmin, xmax);
    std::uniform_real_distribution<double> distY(ymin, ymax);

    for (unsigned int i = 0; i < n; i++) {
        X[i] = distX(generator);
        Y[i] = distY(generator);
    }

    std::vector<double> Z(n, 0.0);

    for (unsigned int i = 0; i < n; i++) {
        Z[i] = std::cos(kx*X[i]) * std::cos(ky*Y[i]);
    }

    //add noise to points
    double sigmaX = 0.01;
    double sigmaY = 0.01;
    double sigmaZ = 0.01;

    std::vector<double> Xnoised = TestUtils::addNoise(X, sigmaX);
    std::vector<double> Ynoised = TestUtils::addNoise(Y, sigmaY);
    std::vector<double> Znoised = TestUtils::addNoise(Z, sigmaZ);
    
    
    double cellSide = 0.5;
    Grid grid(xmin, xmax, ymin, ymax, 0.5);
    
    for (unsigned int i = 0; i < n; i++) {
        Eigen::Vector3d point(X[i], Y[i], Z[i]);
        grid.addPoint(point);
    }
    
    std::vector<std::vector<Cell> > cells = grid.getCells();
    std::vector<Cell> cellsForDBSCAN;
    
    for (unsigned int x = 0; x < grid.getNbCellsAlongX(); x++) {
        for (unsigned int y = 0; y < grid.getNbCellsAlongY(); y++) {
            Cell cell = cells[x][y];
            if(cell.computeFeatures()) {
                cell.gridX = x;
                cell.gridY = y;
                
                cellsForDBSCAN.push_back(cell);
            }
        }
    }
    
    double dbscanEpsilon = 1;
    int dbscanMinimumPoints = 10;
    
    DBSCAN clustering(dbscanEpsilon, dbscanMinimumPoints, cellsForDBSCAN);

    clustering.run();

    std::vector<std::vector<int>> clusters = clustering.getCluster();
    
    //std::cout << "cellsForDBSCAN.size(): " << cellsForDBSCAN.size() << std::endl;
    
    for (int classDBSCAN = 0; classDBSCAN < clusters.size(); classDBSCAN++) {
        //std::cout << "classDBSCAN: " << classDBSCAN << std::endl;
        //std::cout << "clusters[classDBSCAN].size(): " << clusters[classDBSCAN].size() << std::endl;
        for (int cellIdx = 0; cellIdx < clusters[classDBSCAN].size(); cellIdx++) {
            REQUIRE(clusters[classDBSCAN][cellIdx] < cellsForDBSCAN.size());
            //std::cout << "cellIdx: " << cellIdx << std::endl;
            //std::cout << clusters[classDBSCAN][cellIdx] << std::endl;
        }
    }
    
    
    REQUIRE(true);
    
    
    
    
    ClassificationOutput co;
    co.writeClassification("build/test/test.txt", clusters, cellsForDBSCAN);
}

#endif /* OUTPUTDBSCANTEST_HPP */
