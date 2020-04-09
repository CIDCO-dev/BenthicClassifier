/*
 * Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

/* 
 * File:   TestUtils.hpp
 * Author: Jordan McManus <jordan.mcmanus@cidco.ca>
 *
 * Created on January 21, 2020, 12:40 PM
 */

#ifndef TESTUTILS_HPP
#define TESTUTILS_HPP

#include <sstream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>

class TestUtils {
public:

    static std::vector<double> linspace(double a, double b, size_t N) {
        double h = (b - a) / static_cast<double> (N - 1);
        std::vector<double> xs(N);
        typename std::vector<double>::iterator x;
        double val;
        for (x = xs.begin(), val = a; x != xs.end(); ++x, val += h)
            *x = val;
        return xs;
    }

    static std::vector<double> addNoise(std::vector<double> X, double sigma) {
        std::default_random_engine generator;
        std::normal_distribution<double> distribution(0.0, sigma);

        unsigned int n = X.size();
        std::vector<double> xs(n, 0.0);

        double random_number = distribution(generator);
        for (unsigned int i = 0; i < n; i++) {
            xs[i] = X[i] + random_number;
        }

        return xs;
    }

    static void loadPointCloud(std::string filename, std::vector<double> & X, std::vector<double> & Y, std::vector<double> & Z, std::vector<double> & sigmaX, std::vector<double> & sigmaY, std::vector<double> & sigmaZ) {

        std::ifstream file;
        file.open(filename);

        if (!file) {
            std::stringstream errorSS;
            errorSS << "Could not read point cloud from file: " << filename;
            std::cerr << errorSS.str();
            throw std::invalid_argument(errorSS.str());
        } else {

            std::string line;
            unsigned int lineIndex = 0;
            for (; std::getline(file, line);) {
                std::istringstream in(line);
                double x, y, z, sigX, sigY, sigZ;
                in >> x >> y >> z >> sigX >> sigY >> sigZ;

                if (in.fail()) {
                    //line error
                    std::stringstream errorSS;
                    errorSS << "File: " << filename << " has invalid input at line " << lineIndex;
                    std::cerr << errorSS.str();
                    throw std::invalid_argument(errorSS.str());
                } else {
                    X.push_back(x);
                    Y.push_back(y);
                    Z.push_back(z);
                    sigmaX.push_back(sigX);
                    sigmaY.push_back(sigY);
                    sigmaZ.push_back(sigZ);
                }

                ++lineIndex;
            }
            file.close();
        }
    }

};

#endif /* TESTUTILS_HPP */

