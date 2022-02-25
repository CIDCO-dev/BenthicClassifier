#include "DBSCAN-pcl.hpp"
#include <iostream>

DBSCAN::DBSCAN(double eps, int minPts,pcl::PointCloud<pcl::PointXYZ> & cloud,std::vector<SoundingPoint> & soundings) : soundings(soundings),cloud(cloud) {
    //this->n = n;
    this->eps = eps;
    this->minPts = minPts;
    this->size = (int) soundings.size();
    adjPoints.resize(size);
    this->clusterIdx = -1;
}

DBSCAN::~DBSCAN() {
}

void DBSCAN::run() {
    checkNearPoints();

    for (int i = 0; i < size; i++) {
        if (soundings[i].classId != NOT_CLASSIFIED) continue;

        if (isCoreObject(i)) {
            dfs(i, ++clusterIdx);
        } else {
            soundings[i].classId = NOISE;
        }
    }

    cluster.resize(clusterIdx + 1);
    for (int i = 0; i < size; i++) {
        if (soundings[i].classId != NOISE) {
	    //std::cerr << soundings[i].classId << std::endl;
            cluster[soundings[i].classId].push_back(i);
        }
    }
}

void DBSCAN::dfs(int now, int c) {
    soundings[now].classId = c;
    if (!isCoreObject(now)) return;

    for (auto&next : adjPoints[now]) {
        if (soundings[next].classId != NOT_CLASSIFIED) continue;
        dfs(next, c);
    }
}

void DBSCAN::checkNearPoints() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) continue;
            if (getDistance(soundings[i], soundings[j]) <= eps) {
                soundings[i].octave++;
                adjPoints[i].push_back(j);
            }
        }
    }
}

bool DBSCAN::isCoreObject(int idx) {
    return soundings[idx].octave >= minPts;
}

std::vector<std::vector<int> > & DBSCAN::getCluster() {
    return cluster;
}

double DBSCAN::getDistance(SoundingPoint & c1, SoundingPoint & c2) {
    double norm = pow(c1.intensity - c2.intensity,2) +
                  pow(c1.sum - c2.sum,2) +
                  pow(c1.omnivariance - c2.omnivariance,2) +
		  pow(c1.eigenentropy - c2.eigenentropy,2) +
		  pow(c1.anisotropy  -  c2.anisotropy,2)  +
		  pow(c1.planarity - c2.planarity,2) +
                  pow(c1.linearity - c2.linearity,2) +
                  pow(c1.surfaceVariation - c2.surfaceVariation,2) +
                  pow(c1.sphericity - c2.sphericity,2) +
                  pow(c1.verticality - c2.verticality,2) +
                  pow(c1.momentOrder1Axis1 - c2.momentOrder1Axis1,2) +
                  pow(c1.momentOrder1Axis2 - c2.momentOrder1Axis2,2) +
                  pow(c1.momentOrder2Axis1 - c2.momentOrder2Axis1,2) +
                  pow(c1.momentOrder2Axis2 - c2.momentOrder2Axis2,2) +
                  pow(c1.verticalRange - c2.verticalRange,2) +
                  pow(c1.heightBelow - c2.heightBelow,2) +
                  pow(c1.heightAbove - c2.heightAbove,2);
    return sqrt(norm);
}
