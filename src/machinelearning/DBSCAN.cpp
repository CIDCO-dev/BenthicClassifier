#include "DBSCAN.hpp"

DBSCAN::DBSCAN(double eps, int minPts, std::vector<Cell*> & points) : points(points) {
    //this->n = n;
    this->eps = eps;
    this->minPts = minPts;
    this->size = (int) points.size();
    adjPoints.resize(size);
    this->clusterIdx = -1;
}

DBSCAN::~DBSCAN() {
}

void DBSCAN::run() {
    checkNearPoints();

    for (int i = 0; i < size; i++) {
        if (points[i]->class_id != NOT_CLASSIFIED) continue;

        if (isCoreObject(i)) {
            dfs(i, ++clusterIdx);
        } else {
            points[i]->class_id = NOISE;
        }
    }

    cluster.resize(clusterIdx + 1);
    for (int i = 0; i < size; i++) {
        if (points[i]->class_id != NOISE) {
            cluster[points[i]->class_id].push_back(i);
        }
    }
}

void DBSCAN::dfs(int now, int c) {
    points[now]->class_id = c;
    if (!isCoreObject(now)) return;

    for (auto&next : adjPoints[now]) {
        if (points[next]->class_id != NOT_CLASSIFIED) continue;
        dfs(next, c);
    }
}

void DBSCAN::checkNearPoints() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) continue;
            if (getDistance(*points[i], *points[j]) <= eps) {
                points[i]->octave++;
                adjPoints[i].push_back(j);
            }
        }
    }
}

// is idx'th point core object?
bool DBSCAN::isCoreObject(int idx) {
    return points[idx]->octave >= minPts;
}

std::vector<std::vector<int> > & DBSCAN::getCluster() {
    return cluster;
}

double DBSCAN::getDistance(Cell & c1, Cell & c2) {
    Eigen::VectorXd distance = c1.getFeatureVector() - c2.getFeatureVector();
    return distance.norm();
}
