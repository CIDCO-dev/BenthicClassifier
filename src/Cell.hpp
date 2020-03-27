/*
* Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef CELL_HPP
#define CELL_HPP


#include <iostream>
#include <vector>
//#include <memory>

#include <limits>       // std::numeric_limits

#include "math.h"


#include <Eigen/Dense>

#include "Point.hpp"

class Cell
{

public:

    Cell()
        : zMin( std::numeric_limits<double>::max() ),
          zMax( - std::numeric_limits<double>::max() ),
          featuresComputed( false )
    {}

    ~Cell() {}

    void addPoint( Point * point ) {
        points.push_back( point );

        double valueZ = point->getZ();

        if ( valueZ < zMin )
            zMin = valueZ;

        if ( valueZ > zMax )
            zMax = valueZ;
    }

    int getNbOfPoints() {
        return points.size();
    }

    void clear() {
        points.clear();

        zMin = std::numeric_limits<double>::max();
        zMax = - std::numeric_limits<double>::max();

        // TODO: if individual feature variables, reset the variables?

        features.clear();
        featuresComputed = false;
    }

    void display() {
        std::cout << "Cell: " << getNbOfPoints() << " point(s)\n";

        for ( unsigned int count = 0; count < points.size(); count++ ) {

            std::cout << points[ count ]->getX() << " "
                        << points[ count ]->getY() << " "
                        << points[ count ]->getZ() << "\n";
        }

        std::cout << std::endl;
    }

    bool computeFeatures() {

        features.clear();
        featuresComputed = false;

        computeCovarianceMatrix();

        bool OK = computeEigenValuesAndVectors();

        if ( OK == false )
            return false;


        // Article semantic_segmentation_3Dpointcloud_Hackel_2016.pdf
        // p. 179: lambda1 >= lambda2 >= lambda3

        // Eigen: "The eigenvalues are sorted in increasing order."
        // Must switch the order
        double lambda1 = eigenValues( 2 );
        double lambda2 = eigenValues( 1 );
        double lambda3 = eigenValues( 0 );

        // Compute Features

        // ---- Covariance category

        sum = lambda1 + lambda2 + lambda3; //  λ1 + λ2 + λ3
        features.push_back( sum );

        omnivariance = pow( lambda1 * lambda2 * lambda3, 1.0/3 ); // (λ1 · λ2 · λ3) ^ (1/3)
        features.push_back( omnivariance );

        eigenentropy = - ( lambda1 * log( lambda1 )
                        + lambda2 * log( lambda2 )
                        + lambda3 * log( lambda3 ) );        // - SIGMA( λi · ln(λi) )
        features.push_back( eigenentropy );

        anisotropy = ( lambda1 - lambda3 ) / lambda1;          // (λ1 − λ3)/λ1
        features.push_back( anisotropy );

        planarity = ( lambda2 - lambda3 ) / lambda1;          // (λ2 − λ3)/λ1
        features.push_back( planarity );

        linearity = ( lambda1 - lambda2 ) / lambda1;           // (λ1 − λ2)/λ1
        features.push_back( linearity );

        surfaceVariation = lambda3 / ( lambda1 + lambda2 + lambda3 );    // λ3/(λ1 + λ2 + λ3)
        features.push_back( surfaceVariation );

        sphericity = lambda3 / lambda1;          // λ3/λ1
        features.push_back( sphericity );


        // TODO: do the remaining features

        // verticality = 0;


        // ---- Moment category



        // ---- Height category


        // verticalRange = zMax - zMin;


        featuresComputed = true;

        return true;

    }

    std::vector< double > & getFeatures() {
        return features;
    }


private:

    void computeCovarianceMatrix() {

        Eigen::MatrixXd M( points.size(), 3 );

        for ( unsigned int count = 0; count < points.size(); count++ ) {
            M( count, 0 ) = points[ count ]->getX();
            M( count, 1 ) = points[ count ]->getY();
            M( count, 2 ) = points[ count ]->getZ();
        }
        Eigen::MatrixXd centered = M.rowwise() - M.colwise().mean();

        covarianceMatrix = 1.0 / points.size() * ( centered.adjoint() * centered );

//        std::cout << "\ncovarianceMatrix:\n" << covarianceMatrix << "\n" << std::endl;

    }


    bool computeEigenValuesAndVectors() {
        // https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
        // https://eigen.tuxfamily.org/dox/classEigen_1_1SelfAdjointEigenSolver.html
        // "A matrix $ A $ is selfadjoint if it equals its adjoint. For real matrices,
        // this means that the matrix is symmetric: it equals its transpose."

        // "The eigenvalues are sorted in increasing order."


        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver( covarianceMatrix );


        if ( eigensolver.info() != Eigen::Success )
            return false;




        eigenValues = eigensolver.eigenvalues();

        eigenVectors = eigensolver.eigenvectors();


//        std::cout << "\neigensolver.info(): " << eigensolver.info()
//                  << ", eigensolver.info() == Eigen::Success: " << std::boolalpha
//                  << ( eigensolver.info() == Eigen::Success )
//                  << std::noboolalpha << "\n" << std::endl;

        std::cout << "\nEigenvalues:\n" << eigenValues << std::endl;

        std::cout << "\nEigenvectors (columns of the matrix):\n"
                  << eigenVectors << std::endl;

        return true;

    }


// Variables

    std::vector< Point * > points;

    double zMin;
    double zMax;

    Eigen::Matrix3d covarianceMatrix;

    Eigen::Vector3d eigenValues;
    Eigen::Matrix3d eigenVectors;   // Eigen: "The eigenvalues are sorted in increasing order."

    std::vector< double > features;

    bool featuresComputed;

    // TODO: have individual variables or just the vector?

    // Covariance category
    double sum;                 //  λ1 + λ2 + λ3
    double omnivariance;        // (λ1 · λ2 · λ3) ^ (1/3)

    double eigenentropy;        // - SIGMA( λi · ln(λi) )

    double anisotropy;          // (λ1 − λ3)/λ1
    double planarity;           // (λ2 − λ3)/λ1
    double linearity;           // (λ1 − λ2)/λ1
    double surfaceVariation;    // λ3/(λ1 + λ2 + λ3)
    double sphericity;          // λ3/λ1
    // double verticality;

    // Moment category



    // Height category

    double verticalRange;

    // double heightBelow;
    // double heightAbove;




};



#endif
