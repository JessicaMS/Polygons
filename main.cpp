/*
	Collision detection Application by Jessica Seibert
	2/09/09/
	
	
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

//The headers
#include <iostream>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <string>
#include <vector>
#include <math.h>


//Local includes
#include "timer.h"
#include "SDLwindow.h"

#ifndef StarList
	#include "StarList.h"
 #endif
#ifndef StarData
	#include "StarData.h"
#endif
#ifndef Polygon
	#include "Polygon.h"
#endif

//Global types and constants
#define PI 3.14159265
#define WANT_TASTY_MUSHROOM 1 //Always
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define sign(a) (((a)<0) ? -1 : (a)>0 ? 1 : 0)

TTF_Font *font = NULL;


//The screen sttributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;


#ifndef pointtypes
#define pointtypes
struct npoint
{
	int x;
	int y;
};

struct fpoint
{
	double x;
	double y;
};
#endif
static void line32(SDL_Surface *s, 
                   int x1, int y1, 
                   int x2, int y2, 
                   Uint32 color)
{
  int d;
  int x;
  int y;
  int ax;
  int ay;
  int sx;
  int sy;
  int dx;
  int dy;

  Uint8 *lineAddr;
  Sint32 yOffset;

  dx = x2 - x1;  
  ax = abs(dx) << 1;  
  sx = sign(dx);

  dy = y2 - y1;  
  ay = abs(dy) << 1;  
  sy = sign(dy);
  yOffset = sy * s->pitch;

  x = x1;
  y = y1;

  lineAddr = ((Uint8 *)(s->pixels)) + (y * s->pitch);
  if (ax>ay)
  {                      /* x dominant */
    d = ay - (ax >> 1);
    for (;;)
    {
      *((Uint32 *)(lineAddr + (x << 2))) = (Uint32)color;

      if (x == x2)
      {
        return;
      }
      if (d>=0)
      {
        y += sy;
        lineAddr += yOffset;
        d -= ax;
      }
      x += sx;
      d += ay;
    }
  }
  else
  {                      /* y dominant */
    d = ax - (ay >> 1);
    for (;;)
    {
      *((Uint32 *)(lineAddr + (x << 2))) = (Uint32)color;

      if (y == y2)
      {
        return;
      }
      if (d>=0) 
      {
        x += sx;
        d -= ay;
      }
      y += sy;
      lineAddr += yOffset;
      d += ax;
    }
  }
}

void drawpixel(SDL_Surface *screen, int x, int y)
{
	SDL_Rect area;

	area.x = x;
	area.y = y;
	area.w = 1;
	area.h = 1;
	SDL_FillRect(screen, &area, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff ) );

}


bool drawLine(SDL_Surface *screen, npoint Start, npoint Finish)
{
	double dx, dy, step;
	int x, y;

	dx = dy = 0.0;

	step = max( abs(Finish.x-Start.x),abs(Finish.y-Start.y) );
	dx=(Finish.x-Start.x)/step;
	dy=(Finish.y-Start.y)/step;

	x=Finish.y;
	y=Start.y;
	for(int i=0;i<=step;i++)
	{
		  x=x+dx;
		  y=y+dy;
		  drawpixel(screen, x,y);
	}

	return true;
}


bool WritePoint(SDL_Surface *screen, npoint myPoint, bool collide)
{
	SDL_Surface *text;
	SDL_Color textColor = { 0xCC, 0x00, 0xFF };
	char buffer [33];

	if (collide)
		sprintf(buffer, "(%02d, %02d) Yes!", myPoint.x, myPoint.y);//itoa (level,buffer,10);
	else
		sprintf(buffer, "(%02d, %02d) No :(", myPoint.x, myPoint.y);//itoa (level,buffer,10);

	text = TTF_RenderText_Solid( font, buffer, textColor );
	//Get offsets
	SDL_Rect offset;
	offset.x = 30;
	offset.y = 20;
	//Blit
	SDL_BlitSurface( text, &screen->clip_rect, screen, &offset );
	SDL_FreeSurface(text);

	return true;
}

//Initialize whatever you must
bool GameInitialize()
{

	//Initialize all SDL subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
		return false;	
	}

	//Initialize SDL_ttf
	if( TTF_Init() == -1 )
	{
		fprintf(stderr, "TTF_Init() failed: %s\n", TTF_GetError());
		return false;
	}
	
	//Open our font
	//font = TTF_OpenFont(".//Acidd.ttf", 20);
	font = TTF_OpenFont( ".//LESSERCO.ttf", 42 );
	if(!font) 
	{
	    	printf("TTF_OpenFont failed: %s\n", TTF_GetError());
		return false;
	}

	//If everything initialized fine
	return true;
}


//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
//function:  main
//
//The land of tasty mushrooms.  Accepts no arguments. 
//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
int main( int argc, char* args[] )
{

	SDL_Event event;		//The event structure
    	Timer delta;    		//Keeps track of time
	bool quit = false; 		//Quit flag
	bool collide;
	npoint mouseCoord = {0,0};
	int ticks = 0;			//Save the ticks!
	cStarList* myStars = new cStarList(100, 640, 480);;

	delta.start();
	
  	Window myWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, "Collision");
   
	//Initialize SDL and stuff
	if( GameInitialize() == false)
	{
		fprintf(stderr, "GameInitialize() failed: %s\n", SDL_GetError());
		return 1;
	}


	srand(delta.get_ticks());

	cPolygon myPoints;
	myPoints.setPosition(300.0, 200.0);
	fpoint tempPoint;
	tempPoint.x = 0.0;
	tempPoint.y = -1.0;
	myPoints.addPoint(tempPoint);
	tempPoint.x = 1.0;
	tempPoint.y = 1.0;
	myPoints.addPoint(tempPoint);
	tempPoint.x = 0.5;
	tempPoint.y = 1.0;
	myPoints.addPoint(tempPoint);
	tempPoint.x = 0.0;
	tempPoint.y = 0.0;
	myPoints.addPoint(tempPoint);
	tempPoint.x = -0.5;
	tempPoint.y = 1.0;
	myPoints.addPoint(tempPoint);
	tempPoint.x = -1.0;
	tempPoint.y = 1.0;
	myPoints.addPoint(tempPoint);

	myPoints.transformVectors();
	
	collide = false;
	//While the user hasn't quit
	delta.start();
	while(WANT_TASTY_MUSHROOM && !quit)
	{   

		//While there's events to handle
		while( SDL_PollEvent( &event ) )
		{
			myWindow.handle_events(event);
			if( event.key.keysym.sym == SDLK_ESCAPE)//Check for ESCAPE key
			{
				  quit = true;
			}
			if( event.key.keysym.sym == SDLK_SPACE)
			{
				
			}
			if( event.type == SDL_QUIT )		//OS-level quit signal
			{
				quit = true;
			}
			//If the mouse moved
			if( event.type == SDL_MOUSEMOTION )
			{
				//Get the mouse offsets
				mouseCoord.x = event.motion.x;
				mouseCoord.y = event.motion.y;
			}
			
		}

		myStars->MoveStars(mouseCoord.x, mouseCoord.y, ticks);

		SDL_FillRect( myWindow.screen, & myWindow.screen->clip_rect, SDL_MapRGB( myWindow.screen->format, 0x00, 0x00, 0x00 ) );
		SDL_FillRect( myWindow.screen, & myWindow.screen->clip_rect, SDL_MapRGB( myWindow.screen->format, 0x00, 0x00, 0x00 ) );
		myStars->DrawStars( myWindow.screen);

		collide = myPoints.checkCollision(mouseCoord);
		WritePoint(myWindow.screen, mouseCoord, collide);

		myPoints.setAngle( myPoints.getAngle() + (180.0 * ((double)ticks/1000.0)) );
		myPoints.transformVectors();
		myPoints.drawPoints(myWindow.screen);


		//Update the screen
		if( SDL_Flip( myWindow.screen ) == -1 )
		{
			fprintf(stderr, "SDL_Flip() failed: %s\n", SDL_GetError());
			return 1;	
		}

		ticks = delta.get_ticks();
		if (ticks > 2000)
			ticks = 0;
		delta.start();
	} //Farewell, game!


	//myWindow.CleanUp();

	//Clean up
	delete myStars;

	SDL_Quit();

	TTF_CloseFont(font);
	font=NULL;
	TTF_Quit();
	fprintf(stdout, "Normal Quit: %s\n",  SDL_GetError());

	return 0;    
}


