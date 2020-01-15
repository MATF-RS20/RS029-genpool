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
        std::cout << "init label map\n";

        const int range = labelEndX - labelStartX;

        std::cout << "range is: " << range << std::endl;

        for (int i = 0; i < range; i++) {
            
            auto pix = heatMap.get_pixel(labelStartX + i, labelY);
            RGB color = {pix.red, pix.green, pix.blue};
            std::cout << i << "\t" << color[0] << ", " << color[1] << ", " << color[2];
            auto iter = labelMap.find(color);
            
            if (iter != labelMap.end()) {
                std::cout << "\tcontinue\n";
                continue;
            } 

            const double value = (double) i / range;
            labelMap[color] = value;
            std::cout << "\tadding " << std::endl;
        }
        std::cout << labelMap.size() << "/" << range << " colors in label map\n";

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
        std::cout << "starting normalization\n";
        initLabelMap();

        for (auto i = labelMap.begin(); i !=  labelMap.end(); i++) {
            std::cout << i->first[0] << ", " << i->first[1]  << ", " << i->first[2] << '\t' << i->second << std::endl;
        }

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

                    // std::cout << d.second << " \t" << color[0] << ", \t" << color[1] << ", \t" << color[2] << std::endl;

                    heatMap[y][x].red = color[0];
                    heatMap[y][x].green = color[1];
                    heatMap[y][x].blue = color[2];
                    heatMap[y][x].alpha = 255;
                }
            }
        }

        std::cout << "ending normalization\n";
        distCache.clear();

        return heatMap;
    }
};