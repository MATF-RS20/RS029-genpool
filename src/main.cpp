#include <GL/glut.h>
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <cstdlib>
#include <cmath>
#include <cstdio>



//https://www.nongnu.org/pngpp/doc/0.2.0/
#include <png++/png.hpp>

#include "simulation.hpp"
#include "deducedParams.hpp"
#include "ales.hpp"

#include "state.hpp"

#define window_width 1533
#define window_height 767

// -------------------------------TMP state-------------------------------------
png::image<png::rgba_pixel> image("assets/heightEdited.png");
GLvoid *imageData;
Simulation sim(1,100,image);
int n=1;
double suma = 0;
double suma2 = 0;
double suma3 = 0;
double suma4 = 0;
double suma5 = 0;
double suma6 = 0;
double suma7 = 0;
double suma8 = 0;
static GLuint texName;
// --------------------------------------------------------------------------

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
   #ifdef LOG
   std::cout << "inverting colors on image\n";
   #endif
   const int h = image.get_height();
   const int w = image.get_width();

   START_IMAGE_LOOP(image)
      png::rgba_pixel pix = image.get_pixel(j, i);
      image[i][j].blue = 255 - pix.red;
   END_IMAGE_LOOP

   #ifdef LOG
   std::cout << "completed color invertion\n";
   #endif
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
   //invertColors(image);

   #ifdef LOG
   std::cout << "getting height map data\n";
   #endif

   imageData = getImageData(image);
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
if(sim._current_frame<sim._max_duration){
image = sim.nextFrame();
imageData = getImageData(image);
}
   sim._current_frame++;
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
      //green tribe
   case 'g':

     std::cout << "Iteration number " << sim._current_frame << " is current iteration" << std::endl;
   
     for(auto &n:sim.live_people1){
        suma+=n.second.first;
      }
   //Total sum of people in green tribe:
      std::cout << "Total sum of green tribe in " << sim._current_frame << " iteration is: " << suma << std::endl;
      std::cout << "Average number of people per pixel " << suma/sim.live_people1.size() << std::endl;
      std::cout << "Colored area " << sim.live_people1.size() << " pixels" << std::endl; 
      std::cout << std::endl; 
      break;
   //red tribe
   case 'r':
       
      std::cout << "Iteration number " << sim._current_frame << " is current iteration" << std::endl;
   
      for(auto &n:sim.live_people2){
         suma2+=n.second.first;
      }
   //Total sum of people in red tribe:
      std::cout << "Total sum of red tribe in " << sim._current_frame << " iteration is: " << suma2 << std::endl;
      std::cout << "Average number of people per pixel " << suma2/sim.live_people2.size() << std::endl;
      std::cout << "Colored area " << sim.live_people2.size() << " pixels" << std::endl;
      std::cout << std::endl;
      break;
   //blue tribe
   case 'b':

      std::cout << "Iteration number " << sim._current_frame << " is current iteration" << std::endl;
      
      for(auto &n:sim.live_people3){
         suma3+=n.second.first;
      }
   //Total sum of people in blue tribe:
      std::cout << "Total sum of blue tribe in " << sim._current_frame << " iteration is : " << suma3 << std::endl;
      std::cout << "Average number of people per pixel " << suma3/sim.live_people3.size() << std::endl;
      std::cout << "Colored area " << sim.live_people3.size() << " pixels" << std::endl;
      std::cout << std::endl;  
      break;
   //ligth blue tribe
   case 'l':

      std::cout << "Iteration number " << sim._current_frame << " is current iteration" << std::endl;
     
      for(auto &n:sim.live_people4){
         suma4+=n.second.first;
      }
   //Total sum of people in light blue tribe:
      std::cout << "Total sum of light blue tribe in " << sim._current_frame << " iteration is : " << suma4 << std::endl;
      std::cout << "Average number of people per pixel " << suma4/sim.live_people4.size() << std::endl;
      std::cout << "Colored area " << sim.live_people4.size() << " pixels" << std::endl;
      std::cout << std::endl;  
      break;
   //yellow tribe
   case 'y':

      std::cout << "Iteration number " << sim._current_frame << " is current iteration" << std::endl;
   
      for(auto &n:sim.live_people5){
         suma5+=n.second.first;
      }
   //Total sum of people in yellow tribe:
      std::cout << "Total sum of yellow tribe in " << sim._current_frame << " iteration is : " << suma5 << std::endl;
      std::cout << "Average number of people per pixel " << suma5/sim.live_people5.size() << std::endl;
      std::cout << "Colored area " << sim.live_people5.size() << " pixels" << std::endl;
      std::cout << std::endl;  
      break;
   //purple tribe
   case 'p':

      std::cout << "Iteration number " << sim._current_frame << " is current iteration" << std::endl;
     
      for(auto &n:sim.live_people6){
         suma6+=n.second.first;
      }
   //Total sum of people in purple tribe:
      std::cout << "Total sum of purple tribe in " << sim._current_frame << " iteration is : " << suma6 << std::endl;
      std::cout << "Average number of people per pixel " << suma6/sim.live_people6.size() << std::endl;
      std::cout << "Colored area " << sim.live_people6.size() << " pixels" << std::endl;
      std::cout << std::endl;  
      break;
   //coral tribe (India)
   case 'c':

      std::cout << "Iteration number " << sim._current_frame << " is current iteration" << std::endl;
     
      for(auto &n:sim.live_people7){
         suma7+=n.second.first;
      }
   //Total sum of people in coral tribe:
      std::cout << "Total sum of coral tribe in " << sim._current_frame << " iteration is : " << suma7 << std::endl;
      std::cout << "Average number of people per pixel " << suma7/sim.live_people7.size() << std::endl;
      std::cout << "Colored area " << sim.live_people7.size() << " pixels" << std::endl;
      std::cout << std::endl;  
      break;
   //grey tribe
   case 's':

      std::cout << "Iteration number " << sim._current_frame << " is current iteration" << std::endl;
     
      for(auto &n:sim.live_people8){
         suma8+=n.second.first;
      }
   //Total sum of people in grey tribe:
      std::cout << "Total sum of grey tribe in " << sim._current_frame << " iteration is : " << suma8 << std::endl;
      std::cout << "Average number of people per pixel " << suma8/sim.live_people8.size() << std::endl;
      std::cout << "Colored area " << sim.live_people8.size() << " pixels" << std::endl;
      std::cout << std::endl;  
      break;
   default:
      std::cout << "Press one of options [r|g|b]" << std::endl;
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
   //normalizeParams();
   //makeDeducedParams();
   initState();
   //
   glutKeyboardFunc(keyboard);
   registerIOCallbacks();
}

int main(int argc, char **argv)
{ 
  init(argc, argv);
  
  glutMainLoop();

 
  
}

void normalizeParams()
{
   #ifdef LOG
   std::cout << "normalizing params\n";
   #endif

   (Normalizer(moistureMap, 520, 1016, 735, 30))();
   moistureMap.write("assets/moistureNormalized.png");

   (Normalizer(heatMap, 290, 1245, 700, 30))();
   heatMap.write("assets/heatNormalized.png");

   (BMWNormalizer(heightMap))();
   heightMap.write("assets/heightNormalized.png");

}

void makeDeducedParams() {
   #ifdef LOG
   std::cout << "creating deduced param\n";
   #endif

   createAccessabilityMap(accessability);
   accessability.write("assets/accessability.png");

   createSicknessMap(disease);
   disease.write("assets/disease.png");

   createFertilityMap(fertility);
   fertility.write("assets/fertility.png");
}

void blackToRed(HeatMap &map)
{
   #ifdef LOG
   std::cout << "converting black to red\n";
   #endif

   const int h = image.get_height();
   const int w = image.get_width();

   for (int i = 0; i < h; i++)
      for (int j = 0; j < w; j++)
         if (dist(toRGB(map.get_pixel(j, i)), toRGB(png::rgba_pixel(0, 0, 0, 0))) < 24.249)
         {
            pixToRed(map, i, j);
         }
   #ifdef LOG
   std::cout << "converted black to red\n";
   #endif
}

