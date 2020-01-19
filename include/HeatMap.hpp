#pragma once

#include <png++/png.hpp>

#define START_IMAGE_LOOP(img) for (int i = 0; i < img.get_height(); i++) { for (int j = 0; j < img.get_width(); j++) {
#define END_IMAGE_LOOP }}

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