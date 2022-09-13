/*
* Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

/*
 * \author Christian Bouchard, Guillaume Morissette
 */

#ifndef _WIN32
    #include <getopt.h>
#endif

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>

//#include <sstream>

#include <vector>

#include <limits>


#include <Eigen/Dense>

#include "geometry/SoundingPoint.hpp"

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/kdtree/kdtree_flann.h>

#include <math.h>


/**
* Main
*
* @param argc number of CLI arguments
* @param argv value of the CLI arguments
*/
int main( int argc, char* argv[] )
{
	try{
		// Get cli parameters
		int index;

		double radius = 10.0;

		if(argc != 2){
			std::cerr << "Usage: soundings_generate_features radius" << std::endl;
			exit(1);
		}

		// Read point cloud from stdin using georeference output format
		// x y z quality intensity
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

		std::vector<SoundingPoint> soundings;

		float valueX;
		float valueY;
		float valueZ;
	//    float valueQuality;
	//    float valueIntensity;


		std::string line;

		while ( std::getline( std::cin, line ) ) {

		    if (3 == sscanf(line.c_str(), "%f %f %f", &valueX, &valueY, &valueZ)) {
				try{
					//printf("%f %f\n", valueX, valueY);
					cloud->push_back(pcl::PointXYZ(valueX,valueY,valueZ) );
					soundings.push_back(SoundingPoint(0) );
				}
				catch(std::exception & e){
					std::cerr <<  e.what() << std::endl;
				}
			}
		}

		std::cerr << "[+] " << cloud->size() << " points read" << std::endl; 

		//Build kd-tree
		pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
		kdtree.setInputCloud (cloud);

		std::cerr << "[+] Computing geomorphologic features" << std::endl;
		std::cout<<"0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18\n"; // needed for gmm script -> they are columns name
		//Walk the entire point cloud
		for(unsigned int i = 0; i < cloud->points.size(); i++){

		//For every point, do a neighborhood search using kd-tree
		std::vector<int> pointIndex;
		std::vector<float> distances; //squared

		//Covariance matrix
		Eigen::MatrixXd M;

		if ( kdtree.nearestKSearch(cloud->points[i], radius, pointIndex, distances) > 0 ){
			//compute covariance matrix and eigenvalues/eigenvectors
			M.resize(3,pointIndex.size());

			for(unsigned int j=0;j<pointIndex.size();j++){
				M(0,j)=cloud->points[pointIndex[j]].x;
				M(1,j)=cloud->points[pointIndex[j]].y;
				M(2,j)=cloud->points[pointIndex[j]].z;
			}
		}

		Eigen::Vector3d centroid = M.rowwise().mean();

		Eigen::Matrix3d covariance = ((M.colwise() - centroid) * (M.colwise() - centroid).transpose()) / (pointIndex.size() -1);

		Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(covariance);

		if (eigensolver.info() != Eigen::Success){
			std::cerr << "Error eigendecomposition of the local covariance matrix" << std::endl;
			continue; //FIXME: ignore?
		}

		Eigen::Vector3d eigenValues = eigensolver.eigenvalues();

		// Article semantic_segmentation_3Dpointcloud_Hackel_2016.pdf
		// p. 179: Eigenvalues are normalised to sum up to 1, so as to
		// increase robustness against changes in point density

		double eigenValuesSum = eigenValues.sum();
		eigenValues = eigenValues / eigenValuesSum;

		Eigen::Matrix3d eigenVectors = eigensolver.eigenvectors();

		// Eigen: "The eigenvalues are sorted in increasing order."
		// Must switch the order
		double lambda1 = eigenValues(2);
		double lambda2 = eigenValues(1);
		double lambda3 = eigenValues(0);
		
		//std::cerr<< lambda1 <<" "<< lambda2 <<" "<< lambda3 << "\n";
		
		// Covariance features
		
		soundings[i].sum = eigenValuesSum; //  λ1 + λ2 + λ3

		soundings[i].omnivariance = pow(lambda1 * lambda2 * lambda3, 1.0 / 3); // (λ1 · λ2 · λ3) ^ (1/3)
		
		soundings[i].eigenentropy = -(  lambda1 * log(lambda1) + lambda2 * log(lambda2) + lambda3 * log(lambda3)  ); // - SIGMA( λi · ln(λi) )

		soundings[i].anisotropy = (lambda1 - lambda3) / lambda1; // (λ1 − λ3)/λ1

		soundings[i].planarity = (lambda2 - lambda3) / lambda1; // (λ2 − λ3)/λ1

		soundings[i].linearity = (lambda1 - lambda2) / lambda1; // (λ1 − λ2)/λ1
			
		soundings[i].sphericity = lambda3 / lambda1; // λ3/λ1
		
		soundings[i].surfaceVariation = lambda3 / (lambda1 + lambda2 + lambda3); // λ3/(λ1 + λ2 + λ3)
			
		soundings[i].verticality = 1 - abs(eigenVectors(2, 0));




		// Moment & height features
		soundings[i].momentOrder1Axis1 = 0;
		soundings[i].momentOrder1Axis2 = 0;
		soundings[i].momentOrder2Axis1 = 0;
		soundings[i].momentOrder2Axis2 = 0;

		//vertical measurements
		soundings[i].verticalRange = 0;
		soundings[i].heightAbove   = 0;
		soundings[i].heightBelow   = 0;


		//Compute moment and height features
		for(unsigned int j=0;j<pointIndex.size();j++){
			Eigen::Vector3d pi( cloud->points[pointIndex[j]].x , cloud->points[pointIndex[j]].y , cloud->points[pointIndex[j]].z ) ;
			Eigen::Vector3d piMinusCentroid = pi - centroid;

			double piMinusCentroidDotE1 = piMinusCentroid.dot(eigenVectors.col(2));
			double piMinusCentroidDotE2 = piMinusCentroid.dot(eigenVectors.col(1));

			soundings[i].momentOrder1Axis1 += piMinusCentroidDotE1;
			soundings[i].momentOrder1Axis2 += piMinusCentroidDotE2;

			soundings[i].momentOrder2Axis1 += piMinusCentroidDotE1 * piMinusCentroidDotE1;
			soundings[i].momentOrder2Axis2 += piMinusCentroidDotE2 * piMinusCentroidDotE2;

			double height = cloud->points[pointIndex[j]].z  - cloud->points[i].z;

			if(height > soundings[i].heightAbove){
				soundings[i].heightAbove = height;
				soundings[i].verticalRange = soundings[i].heightAbove - soundings[i].heightBelow;
			}

			if(height < soundings[i].heightBelow){
				soundings[i].heightBelow = height;
				soundings[i].verticalRange = soundings[i].heightAbove - soundings[i].heightBelow;
			}
		}
		
		if( !isnan(soundings[i].sum) && !isnan(soundings[i].omnivariance) && !isnan(soundings[i].eigenentropy) &&
			!isnan(soundings[i].anisotropy) && !isnan(soundings[i].planarity) && !isnan(soundings[i].linearity) &&
			!isnan(soundings[i].surfaceVariation) && !isnan(soundings[i].sphericity) && !isnan(soundings[i].verticality) &&
			!isnan(soundings[i].momentOrder1Axis1) && !isnan(soundings[i].momentOrder1Axis2) && !isnan(soundings[i].momentOrder2Axis1) &&
			!isnan(soundings[i].momentOrder2Axis2) && !isnan(soundings[i].verticalRange) && !isnan(soundings[i].heightBelow) &&
			!isnan(soundings[i].heightAbove))
		{
			//print everything
			printf("%.6f,%.6f,%.6f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f\n",
					cloud->points[i].x,
			        cloud->points[i].y,
			        cloud->points[i].z,
			        soundings[i].sum,
			        soundings[i].omnivariance,
			        soundings[i].eigenentropy,
			        soundings[i].anisotropy,
			        soundings[i].planarity,
			        soundings[i].linearity,
			        soundings[i].surfaceVariation,
			        soundings[i].sphericity,
			        soundings[i].verticality,
			        soundings[i].momentOrder1Axis1,
			        soundings[i].momentOrder1Axis2,
			        soundings[i].momentOrder2Axis1,
			        soundings[i].momentOrder2Axis2,
			        soundings[i].verticalRange,
			        soundings[i].heightBelow,
			        soundings[i].heightAbove);
			}
			else{
				std::cerr<<"line "<< i <<" contain NaN" << std::endl;
				// for debugging purposes
				/*
				printf("%.6f,%.6f,%.6f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f,%.12f\n",
					cloud->points[i].x,
			        cloud->points[i].y,
			        cloud->points[i].z,
			        soundings[i].sum,
			        soundings[i].omnivariance,
			        soundings[i].eigenentropy,
			        soundings[i].anisotropy,
			        soundings[i].planarity,
			        soundings[i].linearity,
			        soundings[i].surfaceVariation,
			        soundings[i].sphericity,
			        soundings[i].verticality,
			        soundings[i].momentOrder1Axis1,
			        soundings[i].momentOrder1Axis2,
			        soundings[i].momentOrder2Axis1,
			        soundings[i].momentOrder2Axis2,
			        soundings[i].verticalRange,
			        soundings[i].heightBelow,
			        soundings[i].heightAbove);
			        */
			}
		}
	}
	catch(std::exception & e){
			std::cerr << "Error" << std::endl;
	}
}
