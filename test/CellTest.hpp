#ifndef TESTCELL_HPP
#define TESTCELL_HPP

#include "catch.hpp"

#include <Eigen/Dense>

#include "../src/Cell.hpp"
#include "../src/Point.hpp"
#include "utils/TestUtils.hpp"


TEST_CASE("Test the constructor, adding points, clearing cell")
{

    Cell cell;

    REQUIRE( cell.getNbOfPoints() == 0 );

    Point point1( -64.721366, 1.033044, 14.634260 );
    Point point2( -64.690600, 1.188072, 14.637367 );
    Point point3( -64.659785, 1.343379, 14.640452 );

    cell.addPoint( &point1 );
    cell.addPoint( &point2 );
    cell.addPoint( &point3 );

    REQUIRE( cell.getNbOfPoints() == 3 );

    cell.clear();

    REQUIRE( cell.getNbOfPoints() == 0 );

}

TEST_CASE("Test computation of feature vector")
{
    /*
     * Create test spherical surface  element
     */
    double xmin = 2.0;
    double xmax = 10.0;

    double ymin = 3.0;
    double ymax = 11.0;
    
    double radius = std::sqrt(xmax*xmax + ymax*ymax);


    //number of points
    unsigned int n = 100;

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
        Z[i] = std::sqrt(radius*radius - X[i]*X[i] - Y[i]*Y[i]);
    }

    //add noise to points
    double sigmaX = 0.01;
    double sigmaY = 0.01;
    double sigmaZ = 0.01;

    std::vector<double> Xnoised = TestUtils::addNoise(X, sigmaX);
    std::vector<double> Ynoised = TestUtils::addNoise(Y, sigmaY);
    std::vector<double> Znoised = TestUtils::addNoise(Z, sigmaZ);
    
    Cell cell;
    
    for(unsigned int i = 0; i<n ; i++) {
        cell.addPoint( new Point(Xnoised[i], Ynoised[i], Znoised[i]));
    }
    
    bool computed = cell.computeFeatures();
    
    REQUIRE(computed);
    
    Eigen::VectorXd featureVector = cell.getFeatureVector();
    
    double featureTreshold = 1e-12;
    
    for(unsigned int i = 0; i < cell.getFeatures().size(); i++) {
        REQUIRE(std::abs(featureVector[i] - cell.getFeatures()[i]) < featureTreshold);
    }
}




#endif
