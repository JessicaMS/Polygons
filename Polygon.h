
#include "SDL/SDL.h"
#include <vector>
#include <math.h>
#include "SDL/SDL.h"


#define PI 3.14159265
#define Polygon
#ifndef pointtypes
#define pointtypes 1
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