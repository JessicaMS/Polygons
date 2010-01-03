/*
	Collision detection Application by Jessica Seibert
	2/09/09/
	
	
      _._
    .    ` .
   .       `.
   .________.
    `""``""'
      `  `		
	:  :	We are pillars of society. You can't run your computers, your fashion houses,   
       :  :		your publishing houses, your damn magazines, you can't do anything 
       ;  ;		in culture without psychedelic people in key positions. 
      .  .		- Terence McKenna
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
#include "stars.h"


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

//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
//class:  cPolygon
//
//Contains all of our points in a STL vector
//
//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
class cPolygon
{
private:
	std::vector<fpoint> vPoints;		//Points before transform
	std::vector<npoint> vTransformed;	//Points after vector transformations
	double x, y;				//Positions
	double scale;
	double rotateAngle;

public:
	cPolygon();

	bool addPoint(fpoint myPoint);					//Accessor method for main <vector>
	bool drawPoints(SDL_Surface* screen);
	bool drawPointsScaled(SDL_Surface* screen);
	int Check3(npoint First, npoint Second, npoint Against);	
	//int Check4(fpoint First, fpoint Second, npoint Against);
	bool checkCollision(npoint myPoint);

	double atan2_substitute(double x, double y);
	void transformVectors();
	void setPosition(double x, double y);
	//double getX();
	//double getY();
	void setScale(double scale);
	//double getScale();

	double getAngle();
	void setAngle(double rotateAngle);

	void clearPoints();
	void clearTransformed();

	~cPolygon();

};

cPolygon::cPolygon()
{
	x = 0;
	y = 0;
	scale = 50.0;
	rotateAngle = 0.0;
}

bool cPolygon::addPoint(fpoint myPoint)
{
	vPoints.push_back(myPoint);
	return true;
}

int cPolygon::Check3(npoint F, npoint S, npoint A)
{
	//int sum = abs((int)(F.x - A.x < 0)-(int)(S.x - A.x < 0)) + abs((int)(F.y - A.y < 0) -  (int)(S.y - A.y < 0));
	int sum = abs((F.x - A.x < 0)-(S.x - A.x < 0)) + abs((F.y - A.y < 0) -  (S.y - A.y < 0));
	if (sum == 0)
		return 0;

	if ((F.y - S.y)*(F.x - A.x) - (F.y - A.y)*(F.x - S.x) < 0)
		return sum;
	else return 0-sum;

	return 0;
}
/*
int cPolygon::Check4(fpoint F, fpoint S, npoint A)
{
	npoint nF;
	npoint nS;
	nF.x = (int)(this->x) + (int)F.x;
	nF.y = (int)(this->y) + (int)F.y;
	nS.x = (int)(this->x) + (int)S.x;
	nS.y = (int)(this->y) + (int)S.y;
	//int sum = abs((int)(F.x - A.x < 0)-(int)(S.x - A.x < 0)) + abs((int)(F.y - A.y < 0) -  (int)(S.y - A.y < 0));
	int sum = abs((nF.x - A.x < 0)-(nS.x - A.x < 0)) + abs((nF.y - A.y < 0) -  (nS.y - A.y < 0));
	if (sum == 0)
		return 0;

	if ((nF.y - nS.y)*(nF.x - A.x) - (nF.y - A.y)*(nF.x - nS.x) < 0)
		return sum;
	else return 0-sum;

	return 0;
}*/


bool cPolygon::checkCollision(npoint myPoint)
{
	npoint Last;
	int counter = 0;

	std::vector<npoint>::iterator sipointData;
	sipointData = vTransformed.begin();
	if (sipointData == vTransformed.end())
		return false;

	while (sipointData != vTransformed.end())
	{
		if (sipointData != vTransformed.begin())
			counter += Check3(Last, *sipointData, myPoint);

		Last = *sipointData;
		sipointData++;
	}

	sipointData = vTransformed.begin();
	counter += Check3(Last,*sipointData , myPoint);

	if ((counter % 4) == 0 && counter != 0)
	{
		return true;
	}
	else return false;


}

bool cPolygon::drawPoints(SDL_Surface* screen)
{
	npoint last;
	std::vector<npoint>::iterator sipointData;
	sipointData = vTransformed.begin();
	if (sipointData == vTransformed.end())
	{
		fprintf(stderr, "Error:  No points in vector to draw\n");
		return false;
	}
	while ( sipointData != vTransformed.end() )
	{
		if (sipointData != vTransformed.begin())
		{
			line32(screen, sipointData->x, sipointData->y, last.x, last.y,
					SDL_MapRGB(screen->format, 0xee, 0x00, 0xff ) );
		}
		last.x = sipointData->x;
		last.y = sipointData->y;
		sipointData++;
	}

	sipointData = vTransformed.begin();
	line32(screen, sipointData->x, sipointData->y, last.x, last.y,
		SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00 ) );
	return true;
}

double cPolygon::atan2_substitute(double x, double y)
{
	double angle = 0.0;
	double tmp = 0.0;

	if (x < y)
	{ angle = PI; x = -x; y = -y; }
	// this guarantees that the angle is between -135 and +45 degrees

	if (x < -y)
	{
	angle -= PI/2.0; tmp = x; x = -y; y = tmp;
	}
	// this guarantees that the angle is between -45 and +45

	angle += atan(y/x);

	if (angle > PI)
	angle -= 2.0*PI;
	// fails at 0,0; otherwise is accurate over the entire plane
	return angle;
}


void cPolygon::transformVectors()
{
	double theta = 0.0;
	double hypotenuse = 0.0;

	std::vector<fpoint>::iterator sipointData;
	fpoint tempPoint1;
	npoint tempPoint2;

	clearTransformed();
	for (sipointData = vPoints.begin(); sipointData != vPoints.end(); sipointData++)
	{
		//Adjust by center
		tempPoint1.x = sipointData->x  * scale;
		tempPoint1.y = sipointData->y  * scale;


		//Get angle and distance from center
		
		if (tempPoint1.x == 0.0 && tempPoint1.y == 0.0)
		{
			theta = 0.0;
		}
		else
		{
			theta = atan2_substitute(tempPoint1.x , tempPoint1.y);
		}

		if (tempPoint1.x == 0.0)
		{
			hypotenuse = fabs(tempPoint1.y);
		}
		else
		{
			hypotenuse = tempPoint1.x / cos(theta);
		}
		printf("(%03.02f, %03.02f, %05.02f)\n", tempPoint1.x, tempPoint1.y, hypotenuse);
		theta += (rotateAngle*PI/180.0);

		tempPoint2.x = (int)( cos(theta)*hypotenuse ) + this->x;
		tempPoint2.y = (int)( sin(theta)*hypotenuse ) + this->y;
		vTransformed.push_back(tempPoint2);
		printf("(%03d, %03d)\n", tempPoint2.x, tempPoint2.y);
	}



}

void cPolygon::setPosition(double x, double y)
{
	this->x = x;
	this->y = y;
}

void cPolygon::setScale(double scale)
{
	this->scale = scale;
}


double cPolygon::getAngle()
{
	return this->rotateAngle;
}

void cPolygon::setAngle(double rotateAngle)
{
	while(rotateAngle > 360.0)
		rotateAngle -= 360.0;
	this->rotateAngle = rotateAngle;
}



void cPolygon::clearPoints()
{
	// Delete the nodes from the point vector
	std::vector<fpoint>::iterator sipointData;
	for (sipointData = vPoints.begin(); sipointData != vPoints.end(); sipointData++)
	{
		vPoints.erase(sipointData);
		sipointData--;
	}
}

void cPolygon::clearTransformed()
{
	// Delete the nodes from the point vector
	std::vector<npoint>::iterator sipointData;
	for (sipointData = vTransformed.begin(); sipointData != vTransformed.end(); sipointData++)
	{
		vTransformed.erase(sipointData);
		sipointData--;
	}
}



cPolygon::~cPolygon()
{
	clearPoints();
	clearTransformed();
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
	font = TTF_OpenFont( ".//EnvyCodeR.ttf", 42 );
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


