/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

#ifndef CLASSIFICATIONOUTPUT_HPP
#define CLASSIFICATIONOUTPUT_HPP

#include <vector>
#include <string>
#include "../geometry/Cell.hpp"

class ClassificationOutput {
public:
    ClassificationOutput();
    virtual ~ClassificationOutput();

    void writeClassification(std::string filename, std::vector<std::vector<int> > & cluster, std::vector<Cell> & cells);

private:

};

#endif /* CLASSIFICATIONOUTPUT_HPP */

