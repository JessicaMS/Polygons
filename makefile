#
#`wx-config --cxxflags` `wx-config --libs`
CC = g++

CFLAGS := -Wall `sdl-config --cflags` 
LIBS := `sdl-config --libs` -lSDL_ttf
COMPILE = $(CC) 



all: Linux



Linux: 

	$(COMPILE) main.cpp SDLwindow.cpp timer.cpp -o Execute/collisionLin.out $(CFLAGS) $(LIBS)


Win:

	$(COMPILE) main.cpp SDLwindow.cpp timer.cpp -o Execute/collisionWin.exe -lmingw32 $(CFLAGS) $(LIBS)

OSX: 

	$(COMPILE) main.cpp SDLwindow.cpp timer.cpp -o Execute/collisionOSX.out $(CFLAGS) $(LIBS)

