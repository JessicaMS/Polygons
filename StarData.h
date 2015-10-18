
#define StarData

#include "SDLwindow.h"
#include "SDL/SDL.h"
#include <math.h>
//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
//class:  cStarData
//
//Contains data for each star, as well as a method for moving
//	a star based on mouse coordinates
//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤


//Initializes the data for a star, and assigns the maximum
//height and width it can travel.
class cStarData
{
private:
	int starSize;
	float starVelocity;
	float x,y;
	int width, height;


public:
	bool Initialize(int width, int height);
	bool Initialize();
	bool Draw(SDL_Surface* screen);
	void moveStar(int mouseX, int mouseY, int ticks);
};

