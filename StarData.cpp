#include "StarData.h"


bool cStarData::Initialize(int width, int height)
{
	this->width = width;
	this->height = height;
	int nDistance;
	nDistance = rand() % 20;
	if(nDistance >= 12)
	{
		starSize = 1;
		starVelocity = 1.0;
	}
	else if(nDistance >= 7)
	{
		starSize = 2;
		starVelocity = 2.0;
	}
	else if(nDistance >= 2)
	{
		starSize = 3;
		starVelocity = 4.0;
	}
	else
	{
		starSize = 4;
		starVelocity = 7.5;
	}
	x = (float)(rand() % width);
	y = (float)(rand() % height);

	return true;
}

//Re-initializes, assuming width and height are already defined
bool cStarData::Initialize()
{
	int nDistance;
	nDistance = rand() % 20;
	if(nDistance >= 12)
	{
		starSize = 1;
		starVelocity = 1.0;
	}
	else if(nDistance >= 7)
	{
		starSize = 2;
		starVelocity = 2.0;
	}
	else if(nDistance >= 2)
	{
		starSize = 3;
		starVelocity = 4.0;
	}
	else
	{
		starSize = 4;
		starVelocity = 7.5;
	}
	x = (float)(rand() % width);
	y = (float)(rand() % height);

	return true;
}

//Moves star relative to the mouse coordinates.
//Ticks is used to move depending on how much time has elapsed in milliseconds
//since the last draw.
void cStarData::moveStar(int mouseX, int mouseY, int ticks)
{
	double speed, slopex, slopey;
	float tempx, tempy;
	int d;

	//distance formula
	speed = (mouseX - width/2) * (mouseX - width/2)
		+ (mouseY - height/2) * (mouseY - height/2);
	speed = sqrt(speed);
	d = speed;
	speed /= (100.0);
	
	slopex = fabs(asin((float)(mouseX - width/2) / (float)d));
	slopey = fabs(asin((float)(mouseY - height/2) / (float)d));

	if (mouseX < width/2)
		tempx = (x - slopex*starVelocity*speed*(float(ticks)/100.0));
	else if (mouseX > width/2)
		tempx = (x + slopex*starVelocity*speed*(float(ticks)/100.0));

	if (mouseY < height/2)
		tempy = (y - slopey*starVelocity*speed*(float(ticks)/100.0));
	else if (mouseY > height/2)
		tempy = (y + slopey*starVelocity*speed*(float(ticks)/100.0));
	
	//Check boundaries.
	if (tempx < 0.0)
	{
		Initialize();
		tempx = (float)(width - 1);
		tempy = (float)(rand() % height);
	}
	else if (tempx >= width)
	{
		Initialize();
		tempx = 0.0;
		tempy = (float)(rand() % height);
	}
	else if(tempy < 0.0)
	{
		Initialize();
		tempy = (float)(height - 1);
		tempx = (float)(rand() %  width);
	}
	else if(tempy >= height)
	{
		Initialize();
		tempy = 0.0;
		tempx = (float)(rand() %  width);
	}
	
	x = tempx;
	y = tempy;
	
	

}

//Draw this star to screen
bool cStarData::Draw(SDL_Surface* screen)
{
	SDL_Rect area;

	area.x = x;
	area.y = y;
	area.w = starSize;
	area.h = starSize;
	SDL_FillRect(screen, &area, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff ) );
		
	return true;
}

