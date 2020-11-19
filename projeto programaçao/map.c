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

#define SUJO 0
#define LIMPO 1
#define OBSTACULO 2
#define ROBOT 3
#define OBJETIVO 4

statemap **criar_map(int nSquareW, int nSquareH)
/*creates space in memory to allocate the array 2D (matriz/mapa)*/
{
    statemap **mapa;
    int n, x, y;

    mapa=(statemap**)calloc(nSquareW, sizeof(statemap*));

    for(n=0; n<(nSquareW); n++){
        mapa[n]=(statemap*)calloc(nSquareH, sizeof(statemap));
    }

    if(mapa==NULL){
        printf("Erro ao alocar na memoria");
        exit(0);
    }

    for(x=0; x<nSquareW; x++)
    {
        for(y=0; y<nSquareH; y++)
        {
            mapa[x][y].state=SUJO;
        }
    }
    return mapa;
}


void freemap(int nSquareW, statemap **mapa)
/*clean the local of memory that save the array 2D*/
{
    int aux;

    for(aux=0; aux<nSquareW; aux++){
        free(mapa[aux]);
    }
    free(mapa);
}


statemap **carrega_map(FILE *map, int nSquareW, int nSquareH, int num_obst)
/*reads the position of obstacles from the file and prints in the array 2D that contains the information about the room*/
{
    char linha[STRING_SIZE];
    int i;
    statemap **matriz;
    int x, y;

    matriz=criar_map(nSquareW, nSquareH);

    for(i=0; i<(num_obst) ; i++){
        fgets(linha, STRING_SIZE, map);
        sscanf(linha, "[%d, %d]", &x, &y);
        matriz[x][y].state=OBSTACULO;
    }

    return matriz;
}


void analisa_syntax (FILE *map, int *nSquareW, int *nSquareH, int *num_obstaculos)
/*reads the first line of the file which contains the weight, height and the number of obstacles */
{
    char linha[STRING_SIZE];
    int val1, val2, val3;


    if (fgets (linha, STRING_SIZE, map)==NULL)
    {
        printf("ERRO!, houve problemas na leitura do ficheiro");
        exit(1);
    }

    // Condição que verifica se o nº de elementos na primeira linha são 3, em caso contrário imprime uma mensagem de erro

    if(sscanf( linha, "%d %d %d" , &val1, &val2, &val3) != 3)
    {
        printf ("ERRO!, ficheiro de mapa inválido (argumentos inválidos na primeira linha) ");
        exit(1);
    }

     *nSquareW = val1;
     *nSquareH = val2;
     *num_obstaculos = val3;
}
