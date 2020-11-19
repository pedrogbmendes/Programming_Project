/**
*PROJETO FINAL PROGRAMAÇÃO -
*   Robot de Limpeza - iClean
*
*AUTORES:
*   Pedro Gonçalo Mendes, ist_81046
*   João Guilherme Santos, ist_81126
*
*
* MEEC 2015
* Insti­tuto Superior Tecnico
* Lisboa, 22 de maio de 2015
*/

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

#define STRING_SIZE 1000
#define MAXSTRSIZE 1000
#define OBSTACULO 2
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




int main( int argc, char* argv[] )
{
	int nSquareW = 15;
	int nSquareH = 10;
	int num_obst=0; // nº de obstáculos
	SDL_Event event;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	int width = 0;
	int height = 0;
	int delay = 300;
	int fecho=1;
	int aux=0;
	char linha[STRING_SIZE];
	int num_rob;
  	int xi, yi;
	int xf;
   	int yf;
    	int bateria=0;
    	char caracter;
    	float maxbateria;
    	int quad_perc=0;
    	int pause=0;
    	int add=0;
    	int reset=0;
    	srand((unsigned) time (NULL));
    	statemap **matriz;
    	robot_list *list;
    	robot_list *robot;
    	robot_list *auxi;
    	FILE * map;

    sscanf(argv[2], "%f", &maxbateria);/*recebe o valor maximo que a bateria pode ter */

    map= fopen ( argv[1], "r");

    if(map==NULL){
        printf("Erro na abertura do ficheiro");
        exit(EXIT_FAILURE);
    }

    analisa_syntax (map, &nSquareW, &nSquareH, &num_obst);/*le a primeira linha do ficheiro */
    matriz=carrega_map(map, nSquareW, nSquareH, num_obst);
/*imprime os obstaculos na matriz */

    if(matriz==NULL){
            printf("ERRO ao escrever o array 2D");
        }

    list=begin();/*Inicialização da lista*/
    fgets(linha, STRING_SIZE, map);
    sscanf(linha, "%d", &num_rob);/*le do ficheiro o numero de robots*/

    for(aux=0;aux<num_rob;aux++){
            robot=le_linha(map);/*le o nome, posição inicial e nivel de bateria de um robot*/
            newposition (nSquareH, nSquareW, &xf, &yf);/*posição objectivo */
            matriz[xf][yf].objetivo=OBJETIVO;
            robot->x_objective=xf;
            robot->y_objective=yf;
            robot->squares_covered=0;
            list=add_robot(list, robot);/*escrita na lista dos parametros associados ao novo robot(nome, posiçao, bateria, etc)*/
    }

    cria_matrizrobot(list, nSquareW, nSquareH);/*Cria um mapa da sala para cada robot*/
    fclose(map);
    carrega_robot(matriz, list);/*imprime os robots na matriz*/

    // calculate the window size
	width = SQUARE_SIZE*nSquareW;
	height = SQUARE_SIZE*nSquareH;

	// initialize graphics
	if ( !InitEverything(width, height, &window, &renderer) )
		return -1;

 	while( fecho==1 )
    {
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
			if( event.type == SDL_QUIT )
            {
                fecho=0;
                // quit the program
                // TODO
            }
			else if ( event.type == SDL_KEYDOWN )
			{
               if(add==2){      /*ContinuaÃ§Ã£o da rotina para adicionar novo robot */
                    caracter=(char)event.key.keysym.sym;

                    if(caracter<'a' || caracter>'z'){/*o caracter inserido tem de ser uma letra em minusculo*/
                        printf("Caracter invalido\n");
                        add=1;
                    }
                    sprintf(robot->name,"R%c",caracter);/*guarda numa string o valor de R+o caracter inserido*/

                    for(auxi=list; auxi!=NULL; auxi=auxi->next){
/*percorre o lista e se o nome ja existe volta a começar a rotina de adicionar robots*/
                        if(strcmp( auxi->name, robot->name)==0){
                            printf("Caracter invalido\n");
                            add=1;
                        }
                    }

                    if(add!=1){ /*se o nome é valido segue para a proxima instrução*/
                        printf("Introduza a posiçao\n");
                        add=3;
                    }
                }

                if(add==0)/*desativa o switch quando se escreve o nome para o novo robot*/
                    {
                    switch ( event.key.keysym.sym )
                    {
                        case 'a':
                            if(pause){
                                add=1;
                            }
                        break;

                        case 'q':
                            fecho=0;
                            printf("\nPrograma encerrado\n");
                        break;

                        case 'p':
                            pause=!pause;
                        break;

                        case 'i':
                            reset=1;
                        break;

                        case 'e':
                            print_list(list, nSquareW, nSquareH);
                            /*função que cria o ficheiro com as estatisticas */
                        break;

                        case SDLK_DOWN:
                            if(delay<3000){
                                delay*=1.1;
                            }
                        break;

                        case SDLK_UP:
                            if(delay>50){
                                delay*=0.9;
                            }
                            break;

                        default:
                            break;
                    }
                }
			}
			else if( event.type == SDL_MOUSEBUTTONDOWN)
            {
                if(pause && add==3){/*continuação da rotina inserir uma posição inicial do robot*/
                    SDL_GetMouseState(&xi,&yi);  /*espera pelo evento do rato, ao clicar numa posiçao da matriz guarda a coordena x e y */
                    xi=xi/SQUARE_SIZE;
                    yi=yi/SQUARE_SIZE;

                    if((matriz[xi][yi].state==OBSTACULO) || (matriz[xi][yi].state==ROBOT)) /*posições invalidas para inserir o novo robot (obstaculo e robot)*/
                    {
                        printf("Posiçao inválida.\n");

                    }else{
                        do{
                            robot->bateria=rand()%100;  /*valor random da bateria entre 50 e 100*/
                        }while(robot->bateria<50);
                        matriz[xi][yi].state=ROBOT;
                        robot->x=xi;
                        robot->y=yi;
                        robot->map_robot=criar_map(nSquareW, nSquareH);
                        robot->squares_covered=0;
                        list=add_robot(list, robot); /*adiciona o robot na lista*/
                        newposition (nSquareH, nSquareW, &xf, &yf); /*nova posição objetivo*/
                        matriz[xf][yf].objetivo=OBJETIVO;
                        robot->x_objective=xf;
                        robot->y_objective=yf;
                        num_rob++;
                        add=0; /*sai da rotina de adicionar robot*/
                        carrega_robot(matriz, list);
                        RenderMap(nSquareW, nSquareH, matriz, list, renderer);/*insere o robot  no ecra*/
                    }
                }
            }
        }
        	// add a delay
		SDL_Delay( delay );

        if(add==1){
            robot=new_robot();
            printf("Introduza o caracter do nome do robot\n");
            add=2;
        }

        if(reset==1){/*reinicializa o programa*/
            freemap(nSquareH, matriz); /*apaga a matriz */
            delete_all_list(list); /*apaga a lista*/
            quad_perc=0;
            reset=0;
            /*volta a correr o codigo inicial de forma a ler novamente o ficheiro*/
            map= fopen ( argv[1], "r");

            if(map==NULL){
                printf("Erro na abertura do ficheiro");
                exit(EXIT_FAILURE);
            }

            analisa_syntax (map, &nSquareW, &nSquareH, &num_obst);
            matriz=carrega_map(map, nSquareW, nSquareH, num_obst);

            if(matriz==NULL){
                printf("ERRO ao escrever o array 2D");
            }

            list=begin();
            fgets(linha, STRING_SIZE, map);
            sscanf(linha, "%d", &num_rob);

            for(aux=0;aux<num_rob;aux++){
                robot=le_linha(map);
                newposition (nSquareH, nSquareW, &xf, &yf);
                matriz[xf][yf].objetivo=OBJETIVO;
                robot->x_objective=xf;
                robot->y_objective=yf;
                robot->squares_covered=0;
                list=add_robot(list, robot);
            }
            cria_matrizrobot(list, nSquareW, nSquareH);
            fclose(map);
            carrega_robot(matriz, list);
            RenderMap(nSquareW, nSquareH, matriz, list, renderer);
        }

        if(list==NULL){ /*quando nao existem robot a limpar a simulação é colocada em pausa*/
            pause=1;
        }

        if(!pause){/*se pause nao está ativo corre este codigo*/

            if(end_move(matriz, nSquareW,  nSquareH)==1){
/*verifica quando a simulação acaba*/

                for(robot=list; robot!=NULL; robot=robot->next)
/*percorre todos os robots da lista*/
                {
                    xi=robot->x;
                    yi=robot->y;
                    bateria=robot->bateria;
                    quad_perc=robot->squares_covered;
                    matriz[xi][yi].state=LIMPO;


                    if( xi==robot->x_objective && yi==robot->y_objective )/*se o robot atingiu a posiçao objetivo gera uma nova posiçao para a qual se tenta deslocar*/
                    {
                        matriz[robot->x_objective][robot->y_objective].objetivo=0;
                        matriz[xi][yi].state=LIMPO;

                        do{
                            newposition (nSquareH, nSquareW, &xf, &yf);

                        }while(robot->map_robot[xf][yf].state==LIMPO);

                        matriz[xf][yf].objetivo=OBJETIVO;
                        robot->x_objective=xf;
                        robot->y_objective=yf;
                    }

                    MoveRobot(&xi, &yi, robot->x_objective, robot->y_objective);/*posição seguinte*/

                    while((matriz[xi][yi].state==OBSTACULO) || (matriz[xi][yi].state==ROBOT))/*enquanto a posição seguinte é um obstaculo ou um robot gera uma nova posição objetivo*/
                    {
                        matriz[robot->x_objective][robot->y_objective].objetivo=0;
                        if(matriz[xi][yi].state==OBSTACULO){
                            robot->map_robot[xi][yi].state=OBSTACULO;/*guarda a posição do obstaculo no mapa que o robot conhece*/
                        }
                        do{
                            newposition (nSquareH, nSquareW, &xf, &yf);
                        }while(robot->map_robot[xf][yf].state==LIMPO || robot->map_robot[xf][yf].state==OBSTACULO);

                        matriz[xf][yf].objetivo=OBJETIVO;
                        robot->x_objective=xf;
                        robot->y_objective=yf;
                        xi=robot->x;
                        yi=robot->y;
                        MoveRobot(&xi, &yi, robot->x_objective, robot->y_objective);

                    }
                    quad_perc++;

                    addpoint_list(xi, yi, bateria, quad_perc, robot);/*atualiza os valores de cada robot contidos na lista */
                }
                dec_bat(list, maxbateria); /*decrementa a bateria*/
                list=delete_robotlist(list, matriz);/*apaga o robot da lista se a bateria for menor ou igual a zero*/
                carrega_robot(matriz, list); /*escreve os robots da lista na matriz*/
                RenderMap(nSquareW, nSquareH, matriz, list, renderer);/*atualiza o ecra com a nova matriz*/
            }
        }
    }
    /*no final da simulação*/
   	freemap(nSquareW, matriz);/*apaga a matriz*/
   	delete_all_list(list);/*apaga a lista*/
    	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 1;
}
