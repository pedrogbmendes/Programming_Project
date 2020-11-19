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
#define SQUARE_SIZE 36
#define MARGIN 5

#define SUJO 0
#define LIMPO 1
#define OBSTACULO 2
#define ROBOT 3
#define OBJETIVO 4


robot_list *begin(void)
/*return the list=NULL; the list is empty*/
{
       return NULL;
}


robot_list  *new_robot(void)
/*create a new register in the list to a new robot*/
{
    robot_list *aux = (robot_list*) malloc( sizeof(robot_list) );

    if ( aux == NULL )
    {
        printf("Erro ao reservar a memoria");
        exit(0);
    }

    aux->next = NULL;
    return aux;
}


robot_list *le_linha(FILE * map)
/*read the line of the file  and add it to a new structure*/
{
    char linha[STRING_SIZE];
    robot_list *robot=new_robot();

    fgets(linha, STRING_SIZE ,map);
    sscanf(linha, "%s [%d,%d] %f", robot->name, &robot->x, &robot->y, &robot->bateria);

    if(sscanf(linha, "%s [%d,%d] %f", robot->name, &robot->x, &robot->y, &robot->bateria) != 4){
        printf ("ERRO!, ficheiro de mapa invÃ¡lido ( argumentos invÃ¡lidos)");
        exit(0);
    }
    return robot;
}


robot_list  * add_robot(robot_list *list, robot_list * novo)
 /*add a new robot to a list in alphabetic order*/
{
    robot_list *aux = list;

    if( aux==NULL || (strcmp(aux->name, novo->name)>0)){/*se a condiçao é verdadeira significa que insere no inicio da lista*/
        novo -> next = list;
        return novo;
    }

    while( aux->next != NULL && (strcmp(aux->next->name, novo->name)<0)){/*percorre a lista ate o nome do robot seguinte nao ser maior*/
        aux=aux->next;
    }
    /*insere a estrutura do robot na lista na posição correta, criando os apontadores para ligar novamente a lista*/
    novo -> next = aux -> next ;
    aux -> next = novo;

    return list;
}


robot_list *cria_matrizrobot(robot_list *list, int nSquareW, int nSquareH)
/*allocate memory to create a array 2D for each robot*/
{
    robot_list *aux=list;
    statemap **map;


    for(aux=list; aux!=NULL; aux=aux->next){
        map=criar_map(nSquareW, nSquareH);
        aux->map_robot=map;
    }
    return list;
}


void carrega_robot(statemap **matriz, robot_list *list)
/*insert all robots in array 2D that represent the map of the room*/
{
    robot_list *aux;
    int x;
    int y;
    aux=list;

    while(aux!=NULL){
        x=aux->x;
        y=aux->y;
        matriz[x][y].state=ROBOT;
        strcpy(matriz[x][y].robot, aux->name);
        aux=aux->next;
    }
}


void addpoint_list(int xi, int yi, int bateria, int quad_perc, robot_list *robot)
/*add the new value of x, y, battery, and increment the number of squares covered, and save in array 2D(matriz) of each robot that this positions is clean*/
{
    robot->x=xi;
    robot->y=yi;
    robot->bateria=bateria;
    robot->map_robot[xi][yi].state=LIMPO;
    robot->squares_covered=quad_perc;
}


robot_list *delete_robotlist(robot_list *list, statemap **matriz)
/* delete a robot of the list if the level of battery is less or equal to zero*/
{
    robot_list *aux1=list;
    robot_list *aux2=NULL;

    while(aux1!=NULL){
        if(aux1->bateria<=0){
            matriz[aux1->x_objective][aux1->y_objective].objetivo=0;
            if(aux2==NULL){
                list=aux1->next;
                free(aux1);
                aux1=list;
            }else{
                aux2->next=aux1->next;
                free(aux1);
                aux1=aux2->next;
            }
        }else{
            aux2=aux1;
            aux1=aux1->next;
        }
    }
    return list;
}


robot_list *delete_all_list(robot_list *list)
/*delete all structures of the list */
{
    robot_list *aux=list;
    robot_list *destroy=NULL;

    while(aux!=NULL){
        destroy=aux;
        aux=aux->next;
        free(destroy);
    }
    return list;
}


void print_list(robot_list *list, int nSquareW, int nSquareH)
/* writes the file "estatisticas.txt" that contains the name, the number of cleaned and the number of covered squares, written in the list*/
{
    int n=0, x, y;
    FILE *est;
    robot_list *aux=list;

    est=fopen("estatisticas.txt", "w");

    if(aux==NULL){
        fputs("\nNao existem Robots a efectuar a limpeza\n", est);
    }
    else{
        while(aux!=NULL){

            for(x=0; x<nSquareW; x++)
            {
                for(y=0; y<nSquareH; y++)
                {
                    if((aux->map_robot[x][y].state)==LIMPO){
                        n++;
                    }
                }
            }

            fprintf(est, "\nNome do Robot:  %s\n", aux->name);
            fprintf(est, "\nO Robot limpou %d quadrados\n", n+1);
            fprintf(est, "\nO Robot passou por %d quadrados\n", aux->squares_covered+1);
            fputs("\n==================================================\n", est);
            aux=aux->next;
            n=0;
        }
    }
    fclose(est);
}


void dec_bat(robot_list *list, float maxbateria)
/*decreases a random value to the old value of battery when the robot does a movement*/
{
    robot_list *aux=list;
    float n, dec, dif;

    while(aux!=NULL){
        n=(float) rand()/(float) RAND_MAX;
        dif=maxbateria-0.1;
        dec=n*dif;
        aux->bateria=(aux->bateria) - (dec+0.1);
        aux=aux->next;
    }
}


int end_move(statemap **matriz, int nSquareW, int nSquareH)
/*When all room is clean the simulation stop and the robots stop their movement */
{
    int x, y;

    for(x=0; x<nSquareW; x++)
    {
        for(y=0; y<nSquareH; y++)
        {
            if(matriz[x][y].state==SUJO){
                return 1;
            }
        }
    }
    return 0;
}
