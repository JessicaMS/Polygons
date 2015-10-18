#include "StarList.h"
#ifndef StarData
	#include "StarData.h"
#endif

cStarList::cStarList(int StarCount, int width, int height)
{
	this->height = height;
	this->width = width;

	cStarData _pStarData;// = new Sprite();
	for (int i = 0; i < StarCount; i++)
	{
		(&_pStarData)->Initialize(width, height);

		vStars.push_back(_pStarData);
	}
}


bool cStarList::MoveStars(int mouseX, int mouseY, int ticks)
{
	std::vector<cStarData>::iterator siStarData;
	if (mouseX != width/2 && mouseY !=height/2)
		for (siStarData = vStars.begin(); siStarData != vStars.end(); siStarData++)
		{
			siStarData->moveStar(mouseX, mouseY, ticks);
		}

	return true;
}

bool cStarList::DrawStars(SDL_Surface* screen)
{
	std::vector<cStarData>::iterator siStarData;
	for (siStarData = vStars.begin(); siStarData != vStars.end(); siStarData++)
	{
		(siStarData)->Draw(screen);
	}
	return true;
}


cStarList::~cStarList()
{
	// Delete and remove the sprites in the sprite vector
	std::vector<cStarData>::iterator siStarData;
	for (siStarData = vStars.begin(); siStarData != vStars.end(); siStarData++)
	{
		vStars.erase(siStarData);
		siStarData--;
	}

}