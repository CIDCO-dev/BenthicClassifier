/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

#include "ClassificationOutput.hpp"

#include <fstream>
#include <iostream>
#include <Eigen/Dense>

ClassificationOutput::ClassificationOutput() {
}

ClassificationOutput::~ClassificationOutput() {
}

void ClassificationOutput::writeClassification(std::string filename, std::vector<std::vector<int> > & cluster, std::vector<Cell> & cells) {

    std::ofstream file;
    file.open(filename);

    //write title
    file << "x" << " "
             << "y" << " "
             << "z" << " "
             << "classId" << " "
             << "sum" << " "
            << "omnivariance" << " "
            << "eigenentropy" << " "
            << "anisotropy" << " "
            << "planarity" << " "
            << "linearity" << " "
            << "surfaceVariation" << " "
            << "sphericity" << " "
            << "verticality" << " "
            << "momentOrder1Axis1" << " "
            << "momentOrder1Axis2" << " "
            << "momentOrder2Axis1" << " "
            << "momentOrder2Axis2" << " "
            << "verticalRange" << " "
            << "heightBelow" << " "
            << "heightAbove" << std::endl;

    for (int classDBSCAN = 0; classDBSCAN < cluster.size(); classDBSCAN++) {
        for (int cellIdx = 0; cellIdx < cluster[classDBSCAN].size(); cellIdx++) {
            Cell cell = cells[cluster[classDBSCAN][cellIdx]];
            std::vector< double > features = cell.getFeatures();

            for (int p = 0; p < cell.getPoints().size(); p++) {
                Eigen::Vector3d point = cell.getPoints()[p];

                file << point[0] << " "
                        << point[1] << " "
                        << point[2] << " "
                        << classDBSCAN << " ";
                
                for(int featureIndex = 0; featureIndex < features.size(); featureIndex++) {
                    file << features[featureIndex];
                    if(featureIndex != features.size()-1) {
                        file << " ";
                    }
                }

                file << std::endl;
            }
        }
    }

    file.close();
}

