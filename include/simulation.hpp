#pragma once
#include <GL/glut.h>
#include <iostream>
#include <png++/png.hpp>
#include <RGB.hpp>
#include <state.hpp>
#include<map>
typedef png::image<png::rgba_pixel> HeatMap;
/**
 * 
 */
class Simulation {
public:
    Simulation(
        int current_frame,
        int max_duration, HeatMap picture
    ) : _current_frame(current_frame), _max_duration(max_duration),lastFrame(picture)
    {}

    /**
     * Runs computation for and generates next image of the simulation.
     * Every frame must be generated because n-th frame computationaly depends
     * on n-1- th frame. Also because we may show every 5th frame but save every
     * 100 th.
     */
     HeatMap nextFrame() {
         std::cout <<_current_frame<< " racunam\n";


if(_current_frame==1){
   for (int y = 380; y < 400; y++){
      for (int x = 880; x < 900; x++){
         
         lastFrame[y][x] = png::rgba_pixel( 0,255,0, 255);
         
         double vrednost=colorBijection(toRGB(dis[y][x]));
        
        live_people[std::make_pair(x,y)]=std::make_pair(10,vrednost);
       if(x==880 && y==380){
           std::cout<<vrednost<<"\n";
         }

      }
   }
}
else{

   for(auto &m:live_people){

      int x = m.first.first;
      int y = m.first.second;

      m.second.first=(m.second.first/2*4+m.second.first)*(1-colorBijection(toRGB(dis[y][x])));

      if(x==880 && y==380){
         std::cout<<m.second.first<<" ";
      }

   //top pixel
   auto iter1=live_people.find(std::make_pair(x,y+1));
   if(iter1 == live_people.end()){
      double upAccesssibility=colorBijection(toRGB(acc[y+1][x]));
     
      double upFerlity=colorBijection(toRGB(fer[y+1][x]));

      if (upAccesssibility<0.9 || upFerlity>0.5){
         double vrednost=colorBijection(toRGB(dis[y+1][x]));
         lastFrame[y+1][x] = png::rgba_pixel( 0,255,0, 255);     
      live_people[std::make_pair(x,y+1)]=std::make_pair(100,vrednost);
     
      }
   }
   //bottom pixel
   auto iter2=live_people.find(std::make_pair(x,y-1));
   if(iter2 == live_people.end()){
      double bottomAccesssibility=colorBijection(toRGB(acc[y-1][x]));
      double bottomFerlity=colorBijection(toRGB(fer[y-1][x]));

      if (bottomAccesssibility<0.8 || bottomFerlity>0.5){
         double vrednost=colorBijection(toRGB(dis[y-1][x]));
         lastFrame[y-1][x] = png::rgba_pixel( 0,255,0, 255);     
         live_people[std::make_pair(x,y-1)]=std::make_pair(100,vrednost);
      }
   }

   //left pixel
   auto iter3=live_people.find(std::make_pair(x-1,y));
   if(iter3 == live_people.end()){
      double leftAccesssibility=colorBijection(toRGB(acc[y][x-1]));
      double leftFerlity=colorBijection(toRGB(fer[y][x-1]));

      if (leftAccesssibility<0.8 || leftFerlity>0.5){

         double vrednost=colorBijection(toRGB(dis[y][x-1]));
         lastFrame[y][x-1] = png::rgba_pixel( 0,255,0, 255);      
         live_people[std::make_pair(x-1,y)]=std::make_pair(100,vrednost);
       }
   }
   //right pixel
   auto iter4=live_people.find(std::make_pair(x+1,y));
   if(iter4 == live_people.end()){
      double rightAccesssibility=colorBijection(toRGB(acc[y][x+1]));
      double rightFerlity=colorBijection(toRGB(fer[y][x+1]));

      if (rightAccesssibility<0.9 || rightFerlity>0.5){
        double vrednost=colorBijection(toRGB(dis[y][x+1]));
        lastFrame[y][x+1] = png::rgba_pixel( 0,255,0, 255);  
        live_people[std::make_pair(x+1,y)]=std::make_pair(100,vrednost);
      }
   }

}
}
std::cout<<live_people.size()<<std::endl;
return lastFrame;

}

   
    int _current_frame;
    /**
     * Defines after how many years simulation stops.
     */
    int _max_duration;

    /**
     * One frame represents 100 years. This must not be changed significatntly
     * because rules that simulation utilize are largely dependent on this duration.
     */
    static const int frame_duration = 100;

    png::image< png::rgba_pixel > lastFrame;

  

    HeatMap fer{"assets/fertility.png"};
    HeatMap dis{"assets/disease.png"};
    HeatMap acc{"assets/accessability.png"};

    std::map<std::pair<int,int>,std::pair<double,double>> live_people={};


};