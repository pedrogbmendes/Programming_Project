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

// pre-processor definitions
#define STRING_SIZE 100
#define MAP_SIZE 1000
#define WINDOW_POSX 500
#define WINDOW_POSY 250
#define SQUARE_SIZE 36
#define EXTRASPACE 150
#define MARGIN 5

#define SUJO 0
#define LIMPO 1
#define OBSTACULO 2
#define ROBOT 3
#define OBJETIVO 4

// some global constant variables
const char myName[] = "Pedro Mendes";
const char myName2[]= "Joao Santos";
const char myNumber[] = "IST_81046";
const char myNumber2[] = "IST_81126";


/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Renderer* _renderer)
{
	SDL_Texture *text_IST;
	SDL_Surface *img_IST;
	SDL_Rect boardPos;

	// renders IST Logo
	img_IST = SDL_LoadBMP("ist_logo.bmp");
	if (img_IST == NULL)
	{
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		exit(-5);
	}
	// square where the Logo is placed
	boardPos.x = x;
	boardPos.y = y;
	boardPos.w = img_IST->w;
	boardPos.h = img_IST->h;

	// creates a texture and renders it in the screen
	text_IST = SDL_CreateTextureFromSurface(_renderer, img_IST);
	SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

	// destroy texture and surface
	SDL_DestroyTexture(text_IST);
	SDL_FreeSurface(img_IST);
	return img_IST->h;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *font, SDL_Renderer* _renderer)
{
	SDL_Color color = { 0, 0, 0 };
	SDL_Surface *text_surface;
	SDL_Texture *text_texture;
	SDL_Rect solidRect;

	solidRect.x = x;
	solidRect.y = y;

	// creates a surface with some text
	text_surface = TTF_RenderText_Blended(font,text,color);
	if(!text_surface)
	{
	    printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
	    exit(-5);
	}

	// creates a texture from the surface and renders it !
	text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
	SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
	SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

	// destroy texture and surface
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
	return solidRect.h;
}

/**
 * RenderMap function: Renders the map on the window screen according to their size
 * \param nSquareW number of squares to render (width)
 * \param nSquareH number of squares to render (height)
 * \param pos_robot position of the robot (ID of the square)
 * \param robot_name name of the robot (max. 2 letters)
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderMap(int nSquareW, int nSquareH, statemap **matriz, robot_list *list, SDL_Renderer* _renderer)
{
	TTF_Font *sans;
	TTF_Font *serif;
	SDL_Rect gridPos;
	int i,j, height;
	char robot_name[3];
	// set color of renderer to some color
	SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

	// clear the window
	SDL_RenderClear( _renderer );

	// opens a font style and sets a size
	sans = TTF_OpenFont("FreeSans.ttf", 24);
	serif = TTF_OpenFont("FreeSerif.ttf", 16);
	if(!sans || !serif)
	{
	    printf("TTF_OpenFont: %s\n", TTF_GetError());
	    exit(-5);
	}
	// render the IST Logo
	height = RenderLogo(nSquareW*SQUARE_SIZE, 0, _renderer);

	// render the student name
	height += RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, myName, serif, _renderer);

	// render the student name
	RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, myNumber, serif, _renderer);


    height += RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height+30, myName2, serif, _renderer);


    RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height+30, myNumber2, serif, _renderer);

    //inserts in screen the name and the level of batery of robot
    batery_ecra(list, height, nSquareW, serif, _renderer);


	// grid position
	gridPos.w = SQUARE_SIZE;
	gridPos.h = SQUARE_SIZE;
	gridPos.y = 0;


	// iterate over all squares
	for (i = 0; i < nSquareH; i++)
	{
		gridPos.x = 0;
		for (j = 0; j < nSquareW; j++)
		{
			// writes a dirty square
			if(matriz[j][i].state==SUJO){
			SDL_SetRenderDrawColor( _renderer, 0, 191, 255, 255 );
			SDL_RenderFillRect( _renderer, &gridPos );
			SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
			SDL_RenderDrawRect( _renderer, &gridPos );
			}

            // writes an obstacle in a square
			if(matriz[j][i].state==OBSTACULO){
			SDL_SetRenderDrawColor( _renderer, 181, 181, 181, 255 );
			SDL_RenderFillRect( _renderer, &gridPos );
			SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
			SDL_RenderDrawRect( _renderer, &gridPos );
			}

            //writes a clean square
            if(matriz[j][i].state==LIMPO){
			SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
			SDL_RenderFillRect( _renderer, &gridPos );
			SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
			SDL_RenderDrawRect( _renderer, &gridPos );
			}

            //writes a objective position  in a square to the robot
            if(matriz[j][i].objetivo==OBJETIVO){
			SDL_SetRenderDrawColor( _renderer, 255, 0, 0, 255 );
			SDL_RenderFillRect( _renderer, &gridPos );
			SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
			SDL_RenderDrawRect( _renderer, &gridPos );
			}

             // writes a robot and his name in a square
            if(matriz[j][i].state==ROBOT){
			SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
			SDL_RenderFillRect( _renderer, &gridPos );
			SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
			SDL_RenderDrawRect( _renderer, &gridPos );
			strcpy(robot_name,matriz[j][i].robot);
			RenderText(gridPos.x+MARGIN, gridPos.y, robot_name, sans, _renderer);
			}
			//writes a line between the robot position and objective position that indicates his movement

			gridPos.x += SQUARE_SIZE;

		}
		gridPos.y += SQUARE_SIZE;
	}
	//Draws the line that represents the direction of movement
    DrawLine(list, _renderer);

	// render the changes above
	SDL_RenderPresent( _renderer);

	// destroy everything
	TTF_CloseFont(sans);
	TTF_CloseFont(serif);
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
int InitEverything(int width, int height, SDL_Window** _window, SDL_Renderer** _renderer)
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if ( !InitSDL() )
		return 0;

	if ( !InitFont() )
		return 0;

	window = CreateWindow(width, height);
	if ( window == NULL )
		return 0;

	renderer = CreateRenderer(width, height, window);
	if ( renderer == NULL )
		return 0;

	*_window = window;
	*_renderer = renderer;

	return 1;
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
int InitSDL()
{
	// init SDL library
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf(" Failed to initialize SDL : %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
int InitFont()
{
	// init font library
	if(TTF_Init()==-1)
	{
	    printf("TTF_Init: %s\n", TTF_GetError());
	    return 0;
	}

	return 1;
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
	SDL_Window* window = NULL;
	// init window
	window = SDL_CreateWindow( "iClean", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );

	if ( window == NULL )
	{
		printf("Failed to create window : %s\n", SDL_GetError());
		return NULL;
	}

	return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window* _window)
{
	SDL_Renderer* renderer;
	// init renderer
	renderer = SDL_CreateRenderer( _window, -1, 0 );

	if ( renderer == NULL )
	{
		printf("Failed to create renderer : %s", SDL_GetError());
		return NULL;
	}
	// set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

	return renderer;
}


void DrawLine(robot_list *list, SDL_Renderer* _renderer)
/*draws a line between the robot position and objective position that indicates his movement*/
{

    robot_list *aux=list;

    for(aux=list; aux!=NULL;aux=aux->next){
        SDL_RenderDrawLine(_renderer, ((aux->x)*SQUARE_SIZE+(SQUARE_SIZE/2)), ((aux->y)*SQUARE_SIZE+(SQUARE_SIZE/2)), ((aux->x_objective)*SQUARE_SIZE+(SQUARE_SIZE/2)), ((aux->y_objective)*SQUARE_SIZE+(SQUARE_SIZE/2)));
   }

}


void batery_ecra(robot_list *list, int height, int nSquareW, TTF_Font *_font, SDL_Renderer *_renderer)
/*writes in the screen the name of robots and the level of battery*/
{
    robot_list *aux=list;
    float bat;
    char batery[6];
    char nome[3];

    while(aux!=NULL){
        bat=aux->bateria;
        strcpy(nome, aux->name);
        snprintf(batery, 6, "%f", bat);
        RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height+60, nome, _font, _renderer);
        height += RenderText(nSquareW*SQUARE_SIZE+3*MARGIN+50, height+60, batery, _font, _renderer);
        aux=aux->next;
    }
}
