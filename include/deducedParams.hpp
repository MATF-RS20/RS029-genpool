#pragma once

#include "RGB.hpp"
#include "HeatMap.hpp"
#include "state.hpp"
#include <cmath>
#include <vector>


// CHANGING FOLLOWING MACROS CHANGES RESULTS OF THE SIMULATION. THEY ARE SET
// BY GUESSING
#define LOW_MOISTURE_THRESHOLD .1
#define HIGHT_MOISTURE_THRESHOLD .9
#define HIGH_TEMPERATURE_THRESHOLD .99
#define LOW_TEMPERATURE_THRESHOLD .2

#define EXTREME_MOISTURE_ACCESSABILITY .1
#define EXTREME_TEMPERATURE_ACCESSABILITY .1

/**
 * Calculating any deduced parameter does not need to know about coordinates
 * so implementation only includes logic for computation
 */
double getAccessability(
    double height,
    double moisture,
    double temperature,
    /**
     * Adjecent pixels in the picture. May give any number of relevant neighbours
     * up to all 8. What is relevant is not for this place to decide
     */
    std::vector<double> neighbours
) {
    if (moisture == 0) return 0;
    if (moisture < LOW_MOISTURE_THRESHOLD || moisture > HIGHT_MOISTURE_THRESHOLD)
        return EXTREME_MOISTURE_ACCESSABILITY;
    if (temperature < LOW_TEMPERATURE_THRESHOLD || temperature > HIGH_TEMPERATURE_THRESHOLD) 
        return EXTREME_TEMPERATURE_ACCESSABILITY;

    double differentialSumm = 0;

    for (auto h : neighbours) {
        differentialSumm += abs(h - height);
    }

    return static_cast<double>(1-((differentialSumm) * 90 / neighbours.size()));
}

/**
 * Gets only N-E-W-S and skips NE NW SE SW
 */
std::vector<double> getSimpleNeighbours(int x, int y, HeatMap& map) {
    std::vector<double> retVal;

    auto pix = map.get_pixel(x, y);
    RGB pix_rgb{pix.red, pix.green, pix.blue};

    if (y > 1) {
        auto north = map.get_pixel(x, y - 1);
        retVal.push_back(colorBijection(RGB{north.red, north.green, north.blue}));
    }
    if (y < map.get_height() - 1) {
        auto south = map.get_pixel(x, y + 1);
        retVal.push_back(colorBijection(RGB{south.red, south.green, south.blue}));
    }
    if (x > 1) {
        auto west = map.get_pixel(x - 1, y);
        retVal.push_back(colorBijection(RGB{west.red, west.green, west.blue}));
    }
    if (x < map.get_width() - 1) {
        auto east = map.get_pixel(x + 1, y);
        retVal.push_back(colorBijection(RGB{east.red, east.green, east.blue}));
    }

    return retVal;
}

double getSickness(double moisture, double heat) {
    #define MOISTURE_SICKNESS_THRESHOLD .5
    #define TEMPERATURE_SICKNESS_THRESHOLD .7
    #define ASSERTED_SICKNESS_COEFICIENT .2

    if (moisture < MOISTURE_SICKNESS_THRESHOLD) return ASSERTED_SICKNESS_COEFICIENT;

    double heatCoeficient = 
        heat < TEMPERATURE_SICKNESS_THRESHOLD 
        ? 1 
        : (1 + (heat - TEMPERATURE_SICKNESS_THRESHOLD) 
            / (1 - TEMPERATURE_SICKNESS_THRESHOLD));

    return ASSERTED_SICKNESS_COEFICIENT 
        + (moisture - .5) / .5 * (1 - ASSERTED_SICKNESS_COEFICIENT) / 2 * heatCoeficient;

    #undef MOISTURE_SICKNESS_THRESHOLD
    #undef TEMPERATURE_SICKNESS_THRESHOLD
    #undef ASSERTED_SICKNESS_COEFICIENT
}

HeatMap& createAccessabilityMap(HeatMap& accessability_in_out) {

    #ifdef LOG
    std::cout << "starting accessability map creation\n";
    #endif

    START_IMAGE_LOOP(accessability_in_out)

        auto heightPix = heightMap[i][j];
        double height = colorBijection({heightPix.red, heightPix.green, heightPix.blue});

        auto moisturePix = moistureMap[i][j];
        double moisture = colorBijection({moisturePix.red, moisturePix.green, moisturePix.blue});

        auto heatPix = heatMap[i][j];
        double heat = colorBijection({heatPix.red, heatPix.green, heatPix.blue});

        auto neighbours = getSimpleNeighbours(j, i, heightMap);

        double colorVal = getAccessability(height, moisture, heat, neighbours);
        RGB color = colorVal == 0 ? RGB{255, 0, 0} : colorBijection(1-colorVal);

        accessability_in_out[i][j].red = color[0];
        accessability_in_out[i][j].green = color[1];
        accessability_in_out[i][j].blue = color[2];

    END_IMAGE_LOOP

    return accessability_in_out;
}


HeatMap& createSicknessMap(HeatMap& sickness_in_out) {

    START_IMAGE_LOOP(sickness_in_out)

        auto moisturePix    = moistureMap[i][j];
        double moisture     = colorBijection({moisturePix.red, moisturePix.green, moisturePix.blue});

        auto heatPix    = heatMap[i][j];
        double heat     = colorBijection({heatPix.red, heatPix.green, heatPix.blue});

        double colorVal   = getSickness(moisture, heat);
        RGB color         = colorVal == 0 ? RGB{0, 0, 0} : colorBijection(colorVal);
    
        sickness_in_out[i][j].red   = color[0];
        sickness_in_out[i][j].green = color[1];
        sickness_in_out[i][j].blue  = color[2];

    END_IMAGE_LOOP

    return sickness_in_out;

}

double getFertility(double accessability, double moisture, double heat) {
    return (1 - accessability) * 1/3 + moisture * 1/3 + heat * 1/3;
}

HeatMap& createFertilityMap(HeatMap& fertility_in_out) {

    START_IMAGE_LOOP(fertility_in_out)

        auto accessabilityPix    = accessability[i][j];
        double accessability     = colorBijection({accessabilityPix.red, accessabilityPix.green, accessabilityPix.blue});

        auto moisturePix    = moistureMap[i][j];
        double moisture     = colorBijection({moisturePix.red, moisturePix.green, moisturePix.blue});

        auto heatPix    = heatMap[i][j];
        double heat     = colorBijection({heatPix.red, heatPix.green, heatPix.blue});

        double colorVal   = getFertility(accessability, moisture, heat);
        RGB color         = colorVal == 0 ? RGB{0, 0, 0} : colorBijection(colorVal);
    
        fertility_in_out[i][j].red   = color[0];
        fertility_in_out[i][j].green = color[1];
        fertility_in_out[i][j].blue  = color[2];

    END_IMAGE_LOOP

    return fertility_in_out;

}

