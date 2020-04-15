#ifndef DBSCAN_HPP
#define DBSCAN_HPP

#include <vector>
#include <cmath>
#include "../geometry/Cell.hpp"
#include <Eigen/Dense>

/*
 * The MIT License (MIT)

 * Copyright (c) 2018 glm,jordan,swyoon

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

class Cell;

const int NOISE = -2;
const int NOT_CLASSIFIED = -1;

class DBSCAN {
private:
    //int n
    int minPts;
    double eps;
    std::vector<Cell*> & points;
    int size;
    std::vector<std::vector<int> > adjPoints;
    std::vector<bool> visited;
    std::vector<std::vector<int> > cluster;
    int clusterIdx;

    void dfs (int now, int c);
    void checkNearPoints();
    bool isCoreObject(int idx);
    double getDistance(Cell & c1,Cell & c2);

public:
    DBSCAN(double eps, int minPts, std::vector<Cell*> & points);
    virtual ~DBSCAN();

    void run ();
    std::vector<std::vector<int> > & getCluster();
};

#endif /* DBSCAN_HPP */

