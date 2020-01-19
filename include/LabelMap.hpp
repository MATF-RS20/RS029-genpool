#pragma once

#include <map>

#include "RGB.hpp"

typedef std::map<RGB, double> LabelMap;

LabelMap initLabelMap(
    HeatMap param, 
    int labelStartX, 
    int labelStartY,
    int labelEndX,
    double threshold
);