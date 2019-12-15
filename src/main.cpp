#include <GL/glut.h>
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <cstdlib>

//https://www.nongnu.org/pngpp/doc/0.2.0/
#include <png++/png.hpp>


#define checkImageWidth 640
#define checkImageHeight 640
#define window_width 640
#define window_height 640

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

void makeCheckImage(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         checkImage[i][j][0] = (GLubyte) (rand() % 255);
         checkImage[i][j][1] = (GLubyte) (rand() % 255);
         checkImage[i][j][2] = (GLubyte) (rand() % 255);
         checkImage[i][j][3] = (GLubyte) 100;
      }
   }
}

png::image< png::rgba_pixel > image("assets/globe.png");

static GLuint texName;

void init(void)
{    
   glClearColor(1, 1, 1, 1);
   //glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

   makeCheckImage();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, 
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                checkImage);
}

void reshape(int w, int h)
{
   glViewport(-window_width / 2, -window_height / 2, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   // gluOrtho2D(-1, 1, -1, 1);
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -3.6);
}


GLvoid* getImageData(png::image< png::rgba_pixel > & img) ;

void renderScene(void) {
    /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texName);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1, 1.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1, -1.0, 0.0);

    glEnd(); */


    // glFlush();
    // glDisable(GL_TEXTURE_2D);

   glEnable(GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glRasterPos2d(0, 0);
   // glRasterPos2i(-window_width / 2, -window_height);
   glDrawPixels(image.get_width(), image.get_height(), GL_RGBA,
                GL_UNSIGNED_BYTE, getImageData(image));

   glutSwapBuffers();
   glutPostRedisplay();
}

GLvoid* getImageData(png::image< png::rgba_pixel > & img) 
{
   std::vector<GLubyte>* tmp = new std::vector<GLubyte>();
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

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}


int main(int argc, char **argv)
{
    srand(10);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("BestRsProject");

    init();

    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}

