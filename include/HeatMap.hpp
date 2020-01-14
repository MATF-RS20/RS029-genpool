#pragma once

#include <png++/png.hpp>

typedef png::image<png::rgba_pixel> HeatMap;

//params
void normalizeParams();
/**
 * Normalizes color schemes of input params to a same color scheme.
 * This is done obeying the color label given in the pictures.
 * If some color is not found Euclidean distance is use to determine
 * the closest match. Threshold is used to eliminate seas and oceans.
 * It is the supremum of accepted distances.
 */
void normalizeParam(
    HeatMap param, 
    int labelStartX, 
    int labelStartY,
    int labelEndX,
    double threshold
);