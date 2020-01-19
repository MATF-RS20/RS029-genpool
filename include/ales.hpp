#pragma once
#include <png++/png.hpp>
#include <GL/glut.h>
#include <map>
#include <string>

#include "HeatMap.hpp"
#include "LabelMap.hpp"
#include "Normalizer.hpp"
#include "BMWNormalizer.hpp"

#define pixToStr(pix) '{' << pix.red << ", " << pix.green << ", " << pix.blue << '}'

// state
void initState();

// opengl
void initGlut(void);
void renderFrame();
void initWindow();

// io
void registerIOCallbacks();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

// needed
void engine(void);
GLvoid *getImageData(png::image<png::rgba_pixel> &img);
void init(int argc, char** argv);
void makeDeducedParams();

// test && misc
/**
 * Tests png image lib functions and methods
 */
void testEdit();
inline bool isBlack(const png::rgba_pixel& pix) {
    return pix.blue == 14 && pix.red == 14 && pix.green == 14;
}
inline void pixToRed(HeatMap& map, int w, int h) {
    map[w][h].red = 255;
    map[w][h].blue = 0;
    map[w][h].green = 0;
}

void blackToRed(HeatMap& map);

/**
 * Tests png image lib functions and methods
 */
void invertColors(png::image<png::rgba_pixel> &image);