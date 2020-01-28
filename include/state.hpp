#pragma once

#include "simulation.hpp"
#include "HeatMap.hpp"



/* PARAMS
 * Pictures from google that are heat maps in different color schemes which will
 * later be normalized
 */
HeatMap heightMap{"assets/heightEdited.png"};
HeatMap moistureMap{"assets/precipitationEdited.png"};
HeatMap heatMap{"assets/tmpEdited.png"};

//deduced params

/**
 * Terrain is accessable if its in flat land otherwise it is not. Extreme of any
 * parameter makes terrain unaccessable
 */
HeatMap accessability{"assets/heightEdited.png"};
/**
 * Disease is combination of temperature and moisture.
 */
HeatMap disease{"assets/heightEdited.png"};
/**
 * Soil fertility is combination of moisture and temperature
 */
HeatMap fertility{"assets/heightEdited.png"};

/**
 * These are used to eliminate noise and seas from pictures. Greater value is 
 * more tolerable
 */
const double heightMapThreshold{24.249};
const double heatMapThreshold{24.249};
const double moistureMapThreshold{24.249};
