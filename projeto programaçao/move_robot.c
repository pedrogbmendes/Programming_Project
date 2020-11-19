#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "map.h"
#include "robot_list.h"
#include "graphic.h"
#include "move_robot.h"

void newposition (int nSquareH, int nSquareW, int *xf, int *yf)
/*Calculate a new objective position in the room to the robot */
{
    int x;
    int y;

    x=rand()%nSquareW;
    y=rand()%nSquareH;

    *xf=x;
    *yf=y;
}


void MoveRobot(int *_xc, int *_yc, int xt, int yt)
/*calculate the next position of robot */
{
	int xc, yc;
	double angle;

	// just copy the values
	xc = *_xc;
	yc = *_yc;

	// calculate the angle
	angle = atan2((double)(yc-yt), (double)(xt-xc));

	// calculate the new position
	*_xc = floor(xc + cos(angle)+0.5);
	*_yc = floor(yc - sin(angle)+0.5);
}
