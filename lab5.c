
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
typedef struct {
    int from;       // откуда 
    int to;         // куда
    double w;       // вес ребра
} edge_t;

typedef struct set_t {
    struct set_t *p;
} set_t;


int NS;      // номер узла
set_t *sets; // массив узлов
int NE;      // номер ребра
edge_t *E;   // массив ребер
int NV;      // номера рёбер

// сравнить а и b
int cmpw(edge_t *a, edge_t *b)
{
    if(a->w > b->w ) return 1;
    if(a->w < b->w ) return -1;
    return 0;
}

set_t*
get_set_id(set_t* s)
{
    if(s == s->p )
        return s;
    else {
        set_t *p = get_set_id(s->p);
        s->p = p;
        return p;
    }
}

set_t*
join_sets(set_t *a, set_t *b)
{
    a->p = b;
    return a;
}


void
take_edge(int edge_id)
{
    printf("%d %d %lf\n", E[edge_id].from, E[edge_id].to, E[edge_id].w);
}
int main (int argc, char *argv[]) {

	 int i;
	    double W = 0;
	    scanf("%d%d", &NV, &NE);
	    E = (edge_t*) malloc(NE * sizeof(edge_t));
	    sets = (set_t*) malloc(NV * sizeof(set_t));
	    for(i = 0; i < NE ; i++)
	    {
	        scanf("%d%d%lf", &E[i].from, &E[i].to, &E[i].w);
	    }

	    // сортировка ребер по весу
	    qsort(E, NE, sizeof(edge_t), (int (*)(const void*, const void*)) cmpw);

	    NS = NV;
	    for(i = 0; i < NS ; i++)
	        sets[i].p = &sets[i];

			
		//последовательный
		double t_1=omp_get_wtime();			
	    // Извлечение ребра с минимальным весом
	    for(i=0; NS > 1 && i < NE; i++)
	    {
	        // Если ребро не может быть добавлено, переходим к следующему
	        if ( get_set_id ( &sets[E[i].from]) == get_set_id ( &sets[E[i].to]) )
	                continue;
	        // Добавляем ребро в дерево
	        join_sets ( get_set_id (&sets[E[i].from] ),  get_set_id ( &sets[E[i].to]) );
	        NS--;
	        take_edge(i);
	        W += E[i].w;
	    }
		double t_2=omp_get_wtime();
	    printf("Covering tree weight = %lf\n", W);
	
		double time1=t_2-t_1;

	
		//параллельный
		t_1=omp_get_wtime();
		#pragma omp parallel for	
	    for(i=0; NS > 1 && i < NE; i++)
	    {	        
	        if ( get_set_id ( &sets[E[i].from]) == get_set_id ( &sets[E[i].to]) )
	                continue;	        
	        join_sets ( get_set_id (&sets[E[i].from] ),  get_set_id ( &sets[E[i].to]) );
	        NS--;
	        take_edge(i);
	        W += E[i].w;
	    }
		t_2=omp_get_wtime();
	    printf("Covering tree weight = %lf\n", W);
		
		double time2=t_2-t_1;

	printf("\ntime line=%f\n", time1);
	printf("time parallel=%f\n\n", time2);
	printf("S=%f\n", time1/time2);
	printf("E=%f\n", time1/(time2*2));
	printf("C=%f\n", time2*2);		
}


