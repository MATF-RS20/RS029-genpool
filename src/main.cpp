#include <GL/glut.h>
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <cstdlib>
#include <map>
#include <iterator>

//https://www.nongnu.org/pngpp/doc/0.2.0/
#include <png++/png.hpp>

#define checkImageWidth 640
#define checkImageHeight 640
#define window_width 1533
#define window_height 767
#define full 1533*767*3
unsigned char* buffPixels= (unsigned char *)malloc(sizeof(char)*full);

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

std::map<std::tuple<unsigned char, unsigned char,unsigned char>, double> color_value_map;
png::image<png::rgba_pixel> image("assets/precipitationEdited.png");



// ovo mi sluzi samo da odredim poziciju labele sa slike 
void motion(int x, int y)
{
    y = glutGet( GLUT_WINDOW_HEIGHT ) - y;

    unsigned char pixel[4];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    std::cout << "R: " << (int)pixel[0] <<std:: endl;
    std::cout << "G: " << (int)pixel[1] << std::endl;
    std::cout << "B: " << (int)pixel[2] << std::endl;
    std::cout << std::endl;
    std::cout <<"kordinate" << "x:" <<x << "y:" << y << std::endl;

    
}

void make_map(int x_pocetak, int y, int x_kraj)
{

    y = glutGet(GLUT_WINDOW_HEIGHT) - y;
   color_value_map = {};

   unsigned char pixel[4];

   for (auto i = x_pocetak; i <= x_kraj; i++)
   {
      for(auto y=24;y<42;y++){
      glReadPixels(i, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

      auto kljuc = std::make_tuple(pixel[0],pixel[1],pixel[2]);

      auto iter = color_value_map.find(kljuc);

      if (iter == color_value_map.end())
      {

         color_value_map[kljuc] = (double)(i - x_pocetak) / (x_kraj - x_pocetak);
      }
   }
   }

  for (int x = 98; x < 192; x++)
  {
     for (int y = 284; y < 334 ; y++)
     {
         glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
   auto kljuc = std::make_tuple(pixel[0], pixel[1],pixel[2]);
   auto iter = color_value_map.find(kljuc);
   if (iter == color_value_map.end())
      {

         color_value_map[kljuc] = 1;
      }
        
     }
     
  }
  

   

    


   for (auto m : color_value_map)
   {

      //  std::cout<< std::get<0>(m.first)<<std::endl;
      //   std::cout<< std::get<1>(m.first)<<std::endl;
      //    std::cout<< std::get<2>(m.first)<<std::endl;
       std::cout << "vrednost " << m.second << std::endl;
   }
//---------------------------------------------------------------------------


  

   FILE *imageFile;
   int height=window_height,width=window_width;

   imageFile=fopen("image.ppm","wb");
   if(imageFile==NULL){
      perror("ERROR: Cannot open output file");
      exit(EXIT_FAILURE);
   }

   fprintf(imageFile,"P6\n");               // P6 filetype
   fprintf(imageFile,"%d %d\n",width,height);   // dimensions
   fprintf(imageFile,"255\n");              // Max pixel
 
    int x_kor=1;
    int y_kor=window_height;
    
   for (int i = 0;i < (full) ;i=i+3)
   {
      glReadPixels(x_kor, y_kor, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

      auto kljuc = std::make_tuple(pixel[0],pixel[1],pixel[2]);

      auto iter = color_value_map.find(kljuc);
      //std::cout<<iter->second<< "  ";
      if (iter->second>=0 && iter->second<=1 )
      {
      buffPixels[i]= 0;
   
      buffPixels[i+1]= 0;
      buffPixels[i+2]= iter->second * 255;
        
      }
      else{
       //  std::cout<< "nema ";
      buffPixels[i]= 255;
      buffPixels[i+1]= 0;
      buffPixels[i+2]=0;


      }

      if(x_kor<1533)
      x_kor++;
      else{
         x_kor=1;
         y_kor--;
      }
   

   }

  
   fwrite(buffPixels,1,full,imageFile);
   fclose(imageFile);



//-------------------------------------------------------------------------------
}






















void makeCheckImage(void)
{
   int i, j, c;

   for (i = 0; i < checkImageHeight; i++)
   {
      for (j = 0; j < checkImageWidth; j++)
      {
         checkImage[i][j][0] = (GLubyte)(rand() % 255);
         checkImage[i][j][1] = (GLubyte)(rand() % 255);
         checkImage[i][j][2] = (GLubyte)(rand() % 255);
         checkImage[i][j][3] = (GLubyte)100;
      }
   }
}



static GLuint texName;

void init(void)
{
   glClearColor(1, 1, 1, 1);
   //glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

   // makeCheckImage();
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
   glViewport(-window_width / 2, -window_height / 2, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   // gluOrtho2D(-1, 1, -1, 1);
   gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -3.6);
}

// 1.citam iz slike
// 2. smestam u novu sliku ne u istu
// 3. izdvojiti f za odredjivanje vr iz mape
// 4. naci gresku
// 5. preseliti kod u posebnu klasu
// Andrej misli da je to dosta

// Maja
// 1. pravila implementacija


double findValue(int r, int g,int b){

   auto kljuc = std::make_tuple(r, g, b);

   auto iter = color_value_map.find(kljuc);

   if (iter == color_value_map.end())
   return 0.0;
   else
   return iter->second;
}

/*
GLvoid*  make_new_image(png::image<png::rgba_pixel> &img)
{

   std::vector<GLubyte> *tmp = new std::vector<GLubyte>();

   for (size_t y = img.get_height() - 1; y > 0; --y)
   {
      for (size_t x = 0; x < img.get_width(); ++x)
      {
         unsigned char pixel[4];
         glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
          double alfa= findValue((int)pixel[0], (int)pixel[1], (int)pixel[2]);
         img[y][x].red = (float) alfa * 255;
         img[y][x].green = (float)alfa * 255;
         img[y][x].blue = (float)alfa* 255;
         img[y][x].alpha = (float)alfa;

         tmp->push_back(img[y][x].red);
         tmp->push_back(img[y][x].green);
         tmp->push_back(img[y][x].blue);
         tmp->push_back(img[y][x].alpha);
      }
   }

   return tmp->data();

         // std::cout << iter->second << "  ";
         //png::rgba_pixel p = png::rgba_pixel((double)255, (double)1, (double)1,iter->second);
         // image.set_pixel(x,y,p);

         // image[y][x] = png::rgba_pixel(255,1,1, iter->second);

} */


void make_new_image(){

for(auto x=1100;x<=1150;x++){
      for(auto y=270;y<=357;y++){
       unsigned char pixel[4];
        glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
        auto kljuc = std::make_tuple((int)pixel[0],(int)pixel[1],(int)pixel[2]);
        
        auto iter = color_value_map.find(kljuc);
        if(iter == color_value_map.end()) continue;
        std::cout<<iter->second<<"  " ;
      png::rgba_pixel p = png::rgba_pixel((int)iter->second * 255,  1, 1,(int)iter->second);
      image.set_pixel(x,y,p);
   
  // image[y][x] = png::rgba_pixel(255,1,1,iter->second);
    

      }

}
}


GLvoid *getImageData(png::image<png::rgba_pixel> &img);

void renderScene(void)
{
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

   // glEnable(GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glRasterPos2d(0, 0);
   // glRasterPos2i(-window_width / 2, -window_height);

   glDrawPixels(image.get_width(), image.get_height(), GL_RGBA,
                GL_UNSIGNED_BYTE, getImageData(image));
 
   glutSwapBuffers();
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
   case 'a':
      make_new_image();
      break;

   case 'm':
      make_map(520, 731,1015);
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
    glutPassiveMotionFunc(motion);
   glutMainLoop();



   return 0;
}