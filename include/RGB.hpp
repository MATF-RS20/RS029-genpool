#pragma once

#include <array>
#include <cmath>

typedef std::array<int, 3> RGB;

double dist(const RGB& c1, const RGB& c2){
   return sqrt(pow(c1[0] - c2[0], 2) + pow(c1[1] - c2[1], 2) + pow(c1[2] - c2[2], 2));
}

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

double colorBijection(RGB rgb) {
    const int red = rgb[0];
    const int green = rgb[1];
    const int blue = rgb[2];

    if (!(red == 255 || blue == 255 || green == 255)) return 0;

    if (blue == 255) {
        return .25 * green / 255;
    }
    if (green == 255 && blue > 0) {
        return .25 + .25 * (255 - blue) / 255;
    }

    if (green == 255 && red > 0) {
        return .5 + .25 * red / 255;
    }

    if (red == 255) {
        return .75 + .25 * (255 - green) / 255;
    }
}