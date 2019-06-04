#ifndef trending_topic_H
#define trending_topic_H

#include <stdio.h>
#include "hash.h"

typedef struct trending_topic trending_topic_t;
typedef struct hashtag hashtag_t;

/* Crea una estructura hashtag
 * Pre: recibe el valor alfanumerico del hashtag y un puntero al counMInSketch que lleva
 * cuenta del numero de apariciones.
*/ 
hashtag_t* hashtag_crear(const char *hashtag_char, countMinSketch_t* countMinSketch);

/* Retorna la cantidad de veces que aparece un hashtag de acuerdo a su correspondiente countMinSketch
*/
unsigned int hashtag_repeticiones(hashtag_t* hashtag);

/* Destruye la estructura hashtag.
*/
void hashtag_destruir(hashtag_t* hashtag);

/* Funcion de comparacion para estructuras hashtag
 * Pre: Recibe dos punteros void, los cuales los trata como tipo hashtag_t*
 * Post: Retorna 
 *   menor a 0  si  a > b
 *       0      si  a == b
 *   mayor a 0  si  a < b
 * se considerar como mayor, al de mayor numero de repetisiones, continuado por orden alfabetico.
*/
int hashtag_cmp(const void *a, const void *b);

/* Crea una estructura trending_topic
 * Pre: recibe como parametro, la cantidad de tt que alojara y el tda countMinSketch asociado a los tt que se guardaran.
*/
trending_topic_t* trending_topic_crear(size_t cantidad, countMinSketch_t* countMinSketch);

/* Dado un hashtag_actual busco añadirlo a los k tt.
 * Pre: Recibe el hashtag_actual, con correspondiente countMinSketch, el cual se busca agregar a los k tt en la estructura trendeing_topic. 
 * Post: Retorna true en caso de agregar el hashtag_actual a los k tt, reemplazando al menor de estos y asi manteniendo k tt
 * o en caso de ya pertenecer a los k tt. Retorna false en caso de no corresponda pertenecer a los k tt.
*/
bool trending_topic_agregar(const char *hashtag_actual, trending_topic_t* trending_topic, countMinSketch_t* countMinSketch);

/* Imprime los K trending_topic.
 * Pre: Recibe la estructura trending_topic y un contador "ronda".
 * Post: Imprime los K tt de mayor a menor 1° por numero de repeticiones 2° por orden alfabetico.
*/
void trending_topic_imprimir(trending_topic_t* trending_topic, size_t ronda_numero);

/* Destruye la estructura trending_topic
*/
void trending_topic_destruir(trending_topic_t* trending_topic);

#endif // trending_topic_H