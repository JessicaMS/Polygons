/*
	Stars Header File by Jessica Seibert
	Last recorded update: 1/31/09/
	
	

      _._
    .    ` .
   .       `.
   .________.
    `""``""'
      `  `		
	    :  :	
       :  :		
       ;  ;	
      .  .	
     '  .
    '  .
   .  .
   ;  :
  .    .
 '      ' mh


*/
#include <SDL/SDL.h>
#include <vector>
#include <math.h>
#ifndef StarData
	#include "StarData.h"
#endif
#define StarList

//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
//class:  cStarList
//
//Contains all of our stars in a STL vector
//
//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
class cStarList
{
private:
	std::vector<cStarData> vStars;
	int height, width;


public:

	cStarList(int StarCount, int width, int height);
	bool MoveStars(int mouseX, int mouseY, int ticks);
	bool DrawStars(SDL_Surface* screen);
	~cStarList();

};


