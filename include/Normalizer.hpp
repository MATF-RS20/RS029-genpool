#pragma once

#include "HeatMap.hpp"
#include "LabelMap.hpp"
#include <utility>

typedef double Distance;
typedef double Value;
typedef std::pair<Distance, Value> PixelStat;

class Normalizer {
    HeatMap& heatMap;
    int labelStartX;
    int labelEndX;
    int labelY;
    double threshold;

    LabelMap labelMap;
    std::map<RGB, PixelStat> distCache;

    void initLabelMap() {
        #ifdef LOG
        std::cout << "init label map\n";
        #endif
        const int range = labelEndX - labelStartX;

        #ifdef LOG
        std::cout << "range is: " << range << std::endl;
        #endif

        for (int i = 0; i < range; i++) {
            
            auto pix = heatMap.get_pixel(labelStartX + i, labelY);
            RGB color = {pix.red, pix.green, pix.blue};
            auto iter = labelMap.find(color);
            
            if (iter != labelMap.end()) {
                continue;
            } 

            const double value = (double) i / range;
            labelMap[color] = value;
        }
        #ifdef LOG
        std::cout << labelMap.size() << "/" << range << " colors in label map\n";
        #endif
    }

    /**
     * Distance is minimal distance between two objects
     */
    PixelStat dist(const RGB& c) {

        auto cached = distCache.find(c);
        if(cached != distCache.end()) {
            return cached->second;
        }

        auto i = labelMap.begin();
        double min = ::dist(c, i->first);
        double value = i->second;


        for (i = labelMap.begin(); i != labelMap.end(); i++) {
            auto curDist = ::dist(i->first, c);
            if(min > curDist) {
                min = curDist;
                value = i->second;
            }
        }

        distCache[c] = PixelStat(min, value);

        return PixelStat(min, value);
    }

public:

    Normalizer(
        HeatMap& param,
        int labelStartX,
        int labelEndX,
        int labelY,
        double threshold
    ):  heatMap(param), 
        labelStartX(labelStartX),
        labelEndX(labelEndX),
        labelY(labelY),
        threshold(threshold)
    {
    }

    HeatMap& operator()() {
        #ifdef LOG
        std::cout << "starting normalization\n";
        #endif
        initLabelMap();

        for (size_t y = heatMap.get_height() - 1; y > 0; --y) {
            for (size_t x = 0; x < heatMap.get_width(); ++x) {
                const auto d = dist(toRGB(heatMap[y][x]));
                if (d.first > threshold) {
                    heatMap[y][x].red = 0;
                    heatMap[y][x].green = 0;
                    heatMap[y][x].blue = 0;
                    heatMap[y][x].alpha = 255;
                } else {
                    RGB color = colorBijection(d.second);

                    heatMap[y][x].red = color[0];
                    heatMap[y][x].green = color[1];
                    heatMap[y][x].blue = color[2];
                    heatMap[y][x].alpha = 255;
                }
            }
        }
        #ifdef LOG
        std::cout << "ending normalization\n";
        #endif
        distCache.clear();

        return heatMap;
    }
};