#ifndef ROBOT_LIST_H_INCLUDE
#define ROBOT_LIST_H_INCLUDE

typedef struct robot_list{
    char name[3];
    float bateria;
    int x;
    int y;
    int x_objective;
    int y_objective;
    int squares_covered;
    statemap **map_robot;
    struct robot_list *next;
}robot_list;
/*linked list that contains all necessary data to the simulation*/

robot_list *begin(void);
void apaga_lista(robot_list *);
robot_list  *new_robot(void);
robot_list *le_linha(FILE *);
robot_list  * add_robot(robot_list *, robot_list *);
robot_list *cria_matrizrobot(robot_list *, int , int );
void carrega_robot(statemap **, robot_list *);
void addpoint_list(int, int, int, int, robot_list *);
robot_list *delete_robotlist(robot_list *, statemap **);
robot_list *delete_all_list(robot_list *);
void dec_bat(robot_list *, float);
int end_move(statemap **, int, int);
void print_list(robot_list *, int, int);

#endif // ROBOT_LIST_H_INCLUDE
