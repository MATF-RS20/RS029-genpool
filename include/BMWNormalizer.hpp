#pragma once

#include "HeatMap.hpp"
#include "RGB.hpp"

class BMWNormalizer {
    HeatMap& _param;

public:
    BMWNormalizer(HeatMap& param) : _param(param) {}

    void operator() () {

        #ifdef LOG
        std::cout << "starting BMW normalization\n";
        #endif
        RGB black {0, 0, 0}, white {255, 255, 255};

        const double totalNorm = dist(black, white);

        START_IMAGE_LOOP(_param)
            RGB cur {_param[i][j].red, _param[i][j].green, _param[i][j].blue};

            RGB color = colorBijection(dist(black, cur) / totalNorm);

            _param[i][j].red = color[0];
            _param[i][j].green = color[1];
            _param[i][j].blue = color[2];

        END_IMAGE_LOOP

        #ifdef LOG
        std::cout << "ending BMW normalization\n";
        #endif
    }
};