#ifndef MAP_H_INCLUDE
#define MAP_H_INCLUDE

typedef struct statemap{
    int state;
    char robot[3];
    int x;
    int y;
    int objetivo;
}statemap;
/*structure of the type of array 2D(matriz), all array 2D are composed for this variables */

statemap **criar_map(int , int);
void freemap(int , statemap **);
statemap **carrega_map(FILE *, int , int , int );
void analisa_syntax (FILE *, int *, int *, int *);

#endif // MAP_H_INCLUDE
