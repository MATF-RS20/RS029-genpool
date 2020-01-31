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
        int max_duration, HeatMap picture)
        :_current_frame(current_frame), _max_duration(max_duration),lastFrame(picture)
    {}



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

   png::image<png::rgba_pixel> lastFrame;

    HeatMap fer{"assets/fertility.png"};
    HeatMap dis{"assets/disease.png"};
    HeatMap acc{"assets/accessability.png"};

    std::map<std::pair<int,int>,std::pair<double,double>> live_people1={};
    std::map<std::pair<int,int>,std::pair<double,double>> live_people2={};
    std::map<std::pair<int,int>,std::pair<double,double>> live_people3={};
   /**
    * Make first generation of tribes
    */
   void makeTribes(){

     for (int y = 390; y < 400; y++){
      for (int x = 885; x < 895; x++){
         
         lastFrame[y][x] = png::rgba_pixel(0,255,0, 255);
         lastFrame[y+25][x-20] = png::rgba_pixel( 255,0,0, 255);
         lastFrame[y+18][x-10] = png::rgba_pixel( 0,0,255, 255);
         
         double vrednost1=colorBijection(toRGB(dis[y][x]));
         double vrednost2=colorBijection(toRGB(dis[y][x]));
         double vrednost3=colorBijection(toRGB(dis[y][x]));
        
        live_people1[std::make_pair(x,y)]=std::make_pair(10,vrednost1);
        live_people2[std::make_pair(x-20,y+25)]=std::make_pair(10,vrednost2);
        live_people3[std::make_pair(x-10,y+18)]=std::make_pair(10,vrednost3);
         }
      }

   }

     /**
       * Test if current tribe is winner if one or more
       * tribes want the same teritory
       */
    void testTheExpansion(int x,int y,std::map<std::pair<int,int>,std::pair<double,double>> &change,
    std::map<std::pair<int,int>,std::pair<double,double>> change_copy,
    std::map<std::pair<int,int>,std::pair<double,double>> first,
     std::map<std::pair<int,int>,std::pair<double,double>> second,double num){
  
      auto iter=change_copy.find(std::make_pair(x,y));
      if(iter == change_copy.end()){
         
         auto find_first=first.find(std::make_pair(x,y));
         auto find_second=second.find(std::make_pair(x,y));

         double upAccesssibility=colorBijection(toRGB(acc[y][x]));
         double upFerlity=colorBijection(toRGB(fer[y][x]));

      if (upAccesssibility<0.9 || upFerlity>0.4){
     
         bool win = true;
         if(find_first!=first.end()){
            win &= find_first->second.first < num;
         }
         if(find_second!=second.end()){
            win &= find_second->second.first < num;
         }

         if(win == true){
         double vrednost=colorBijection(toRGB(dis[y][x]));

        if(change==live_people1){
        lastFrame[y][x] = png::rgba_pixel( 0,255,0, 255);
        }else if (change==live_people2){
        lastFrame[y][x] = png::rgba_pixel( 255,0,0, 255);
        }else{
        lastFrame[y][x] = png::rgba_pixel( 0,0,255, 255);
        }
         
         change[std::make_pair(x,y)]=std::make_pair(100,vrednost);}
       
      }

     
      }
   }
     /**
       * In each generation tribes chack if they can expands N,W,S,E
       */
    void expandingTribes( std::map<std::pair<int,int>,std::pair<double,double>> &change
                          ,std::map<std::pair<int,int>,std::pair<double,double>> change_copy
                         ,std::map<std::pair<int,int>,std::pair<double,double>> first
                         ,std::map<std::pair<int,int>,std::pair<double,double>> second){
  
      for(auto &m:change_copy){

      int x = m.first.first;
      int y = m.first.second;

      m.second.first=(m.second.first/2*4+m.second.first)*(1-colorBijection(toRGB(dis[y][x])));
      double currentTribes_num=m.second.first;
      
      testTheExpansion(x,y+1,change,change_copy,first,second,currentTribes_num);
      testTheExpansion(x,y-1,change,change_copy,first,second,currentTribes_num);
      testTheExpansion(x-1,y,change,change_copy,first,second,currentTribes_num);
      testTheExpansion(x+1,y,change,change_copy,first,second,currentTribes_num);
      

     
   }
   }
   /**
     * Runs computation for and generates next image of the simulation.
     * Every frame must be generated because n-th frame computationaly depends
     * on n-1- th frame.
     */
   HeatMap nextFrame(){

      std::cout <<_current_frame<< " racunam\n";


      if (_current_frame == 1){
         makeTribes();    
      }
      else{
      expandingTribes(live_people1,live_people1,live_people2,live_people3);
      expandingTribes(live_people2,live_people2,live_people1,live_people3);
      expandingTribes(live_people3,live_people3,live_people1,live_people2);

     


      }
     std::cout<<live_people1.size()<<std::endl;
     std::cout<<live_people2.size()<<std::endl;
     std::cout<<live_people3.size()<<std::endl;

     return lastFrame;

   }



};