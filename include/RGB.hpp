#pragma once

#include <array>
#include <cmath>

typedef std::array<int, 3> RGB;

double dist(const RGB& c1, const RGB& c2);

RGB toRGB(const png::rgba_pixel& p) {
    return {p.red, p.green, p.blue};
}


RGB colorBijection(double val) {
    RGB rgb{0, 0, 0};

    if (val < .25) rgb[2] = 255;
    if (val >= .25 && val < .5) rgb[2] = abs(.5 - val) / .25 * 255;


    if (val > .75) rgb[0] = 255;
    if (val >= .5 && val < .75) rgb[0] = abs(val - .5) / .25 * 255;

    if (val >= .25 && val < .75) rgb[1] = 255;
    if (val < .25) rgb[1] = val / .25 * 255;
    if (val > .75) rgb[1] = abs(1 - val) / .25 * 255;

    return rgb;
}