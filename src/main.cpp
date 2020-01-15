#include <GL/glut.h>
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <cstdlib>
#include <cmath>

//https://www.nongnu.org/pngpp/doc/0.2.0/
#include <png++/png.hpp>

#include "simulation.cpp"

#include "ales.hpp"

#define window_width 1533
#define window_height 767

// -------------------------------VAZNO-----------------------------------------
Simulation sim{100, 100'000};

//params
HeatMap heightMap{"assets/heightEdited.png"};
HeatMap moistureMap{"assets/precipitationEdited.png"};
HeatMap heatMap{"assets/tmpEdited.png"};

const double heightMapThreshold{24.249};
const double heatMapThreshold{24.249};
const double moistureMapThreshold{24.249};

// -------------------------------TMP state-------------------------------------
png::image<png::rgba_pixel> image("assets/precipitationEdited.png");
GLvoid *imageData;

// --------------------------------------------------------------------------

static GLuint texName;
/**
 * Tests png image lib functions and methods
 */
void testEdit()
{
   for (int y = 500; y < 700; y++)
   {
      for (int x = 500; x < 700; x++)
      {
         image[y][x] = png::rgba_pixel(255, 0, 0, 255);
      }
   }
}
/**
 * Tests png image lib functions and methods
 */
void invertColors(png::image<png::rgba_pixel> &image)
{

   std::cout << "inverting colors on image\n";

   const int h = image.get_height();
   const int w = image.get_width();

   for (int i = 0; i < h; i++)
   {
      for (int j = 0; j < w; j++)
      {
         png::rgba_pixel pix = image.get_pixel(j, i);
         image[i][j].blue = 255 - pix.red;
      }
   }

   std::cout << "completed color invertion\n";
}

void initGlut(void)
{
   glClearColor(1, 1, 1, 1);
   //glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_NEAREST);
   /* glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, 
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                checkImage); */
}

void initState()
{
   invertColors(image);
   std::cout << "getting height map data\n";
   imageData = getImageData(heatMap);
}
void reshape(int w, int h)
{
   glViewport(-window_width / 2, -window_height / 2, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   // gluOrtho2D(-1, 1, -1, 1);
   gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -3.6);
}

void renderFrame()
{
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glRasterPos2d(0, 0);
   // glRasterPos2i(-window_width / 2, -window_height);
   glDrawPixels(image.get_width(), image.get_height(), GL_RGBA,
                GL_UNSIGNED_BYTE, imageData);

   glutSwapBuffers();
}

void engine(void)
{

   sim.nextFrame();
   renderFrame();

   glutPostRedisplay();
}

GLvoid *getImageData(png::image<png::rgba_pixel> &img)
{

   std::vector<GLubyte> *tmp = new std::vector<GLubyte>();
   for (size_t y = img.get_height() - 1; y > 0; --y)
   {
      for (size_t x = 0; x < img.get_width(); ++x)
      {
         tmp->push_back(img[y][x].red);
         tmp->push_back(img[y][x].green);
         tmp->push_back(img[y][x].blue);
         tmp->push_back(img[y][x].alpha);
      }
   }

   return tmp->data();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 27:
      exit(0);
      break;
   default:
      break;
   }
}

void registerIOCallbacks()
{
   /* This line is wicked. We don't want to use glut timer func since we want
    * as much frames as possible. Not mandatory for this kind of application
    * but nice shortcut. We register engine in place of render. Engine renders
    * and manages state. Also engine calls it self by invoking post redisplay
    * which gives oportunity for other io functions to execute
    */
   glutDisplayFunc(engine);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
}

void initWindow()
{
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(window_width, window_height);
   glutCreateWindow("BestRsProject");
}

/**
 * Init everything 
 */
void init(int argc, char **argv)
{
   // init the lib
   glutInit(&argc, argv);
   srand(10);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
   initWindow();
   initGlut();
   normalizeParams();
   initState();
   registerIOCallbacks();
}

int main(int argc, char **argv)
{
   init(argc, argv);
   glutMainLoop();
}

void normalizeParams()
{
   //(Normalizer(moistureMap, 520, 1016, 735, 30))();
   (Normalizer(heatMap, 290, 1245, 700, 30))();
   //blackToRed(heightMap);
}

void blackToRed(HeatMap &map)
{
   std::cout << "converting black to red\n";

   const int h = image.get_height();
   const int w = image.get_width();

   for (int i = 0; i < h; i++)
      for (int j = 0; j < w; j++)
         if (dist(toRGB(map.get_pixel(j, i)), toRGB(png::rgba_pixel(0, 0, 0, 0))) < 24.249)
         {
            pixToRed(map, i, j);
         }

   std::cout << "converted black to red\n";
}

double dist(const RGB& c1, const RGB& c2){
   return sqrt(pow(c1[0] - c2[0], 2) + pow(c1[1] - c2[1], 2) + pow(c1[2] - c2[2], 2));
}


