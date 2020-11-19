#ifndef GRAPHIC_H_INCLUDE
#define GRAPHIC_H_INCLUDE

//declaration of the graphic functions of SDL2
int InitEverything(int , int , SDL_Window** , SDL_Renderer** );
int InitSDL();
int InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window* );
void RenderMap(int , int  , statemap **, robot_list *, SDL_Renderer* );
int RenderText(int , int , const char* , TTF_Font *, SDL_Renderer* );
int RenderLogo(int , int , SDL_Renderer* );
void DrawLine(robot_list *, SDL_Renderer* );
void batery_ecra(robot_list *, int, int, TTF_Font *, SDL_Renderer *);

#endif // GRAPHIC_H_INCLUDE
