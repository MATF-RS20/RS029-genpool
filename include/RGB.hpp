#pragma once

#include <array>

typedef std::array<int, 3> RGB;

double dist(const RGB& c1, const RGB& c2);

RGB toRGB(const png::rgba_pixel& p) {
    return {p.red, p.green, p.blue};
}