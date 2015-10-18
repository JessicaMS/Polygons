#ifndef Polygon
	#include "Polygon.h"
#endif

#define sign(a) (((a)<0) ? -1 : (a)>0 ? 1 : 0)

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
			// DrawTools* myDraw = new DrawTools();
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

