/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

 /*
 * \author Christian Bouchard, jordan
 */

#ifndef CELL_HPP
#define CELL_HPP

#include <iostream>
#include <vector>
//#include <memory>

#include <limits>       // std::numeric_limits

#include "math.h"

#include <Eigen/Dense>


class Cell {
public:
    Cell();
    virtual ~Cell();

    /*
     * Baked fresh in Hell ...
     */
    int class_id = -1;
    int octave = 0;
    int gridX = -1;
    int gridY = -1;
    /*
     * enjoy them while they're hot!
     * Kill this with fire plox
     */

    void addPoint( Eigen::Vector3d * point );

    void clear();
    void display();
    bool computeFeatures();

    // Get / Set
    std::vector< double > & getFeatures()		{return features;};
    Eigen::VectorXd & getFeatureVector()		{return featureVector;};
    bool areFeaturesComputed()				{return featuresComputed;};
    std::vector< Eigen::Vector3d * > & getPoints()	{return points;};

private:

    void computeCovarianceMatrix();
    bool computeEigenValuesAndVectors();

    std::vector<Eigen::Vector3d *> points;

    double zMin;
    double zMax;

    Eigen::Vector3d centroid;

    //Covariance matrix stuff
    Eigen::Matrix3d covarianceMatrix;

    Eigen::Vector3d eigenValues;
    Eigen::Matrix3d eigenVectors;   // Eigen: "The eigenvalues are sorted in increasing order."

    // TODO: have individual variables or just the vector?

    // Article semantic_segmentation_3Dpointcloud_Hackel_2016.pdf
    // p. 179: eigenvalues lambda1 >= lambda2 >= lambda3

    std::vector< double > features;
    Eigen::VectorXd featureVector;

    bool featuresComputed;


    // Covariance matrix features
    double sum;                 //  λ1 + λ2 + λ3
    double omnivariance;        // (λ1 · λ2 · λ3) ^ (1/3)

    double eigenentropy;        // - SIGMA( λi · ln(λi) )

    double anisotropy;          // (λ1 − λ3)/λ1
    double planarity;           // (λ2 − λ3)/λ1
    double linearity;           // (λ1 − λ2)/λ1
    double surfaceVariation;    // λ3/(λ1 + λ2 + λ3)
    double sphericity;          // λ3/λ1

    // Verticality features
    double verticality;         // 1 − | < [0 0 1], e3i >|      where <u,v> is the dot product

    // Moment features
    double momentOrder1Axis1;   // SIGMA(  < pi - p, e1 > )        p is centroid of cell
    double momentOrder1Axis2;   // SIGMA(  < pi - p, e2 > )        p is centroid of cell
    double momentOrder2Axis1;   // SIGMA(  < pi - p, e1 >^2 )      p is centroid of cell
    double momentOrder2Axis2;   // SIGMA(  < pi - p, e2 >^2 )      p is centroid of cell

    // Height features
    double verticalRange;
    double heightBelow;
    double heightAbove;
};

#endif /* CELL_HPP */
