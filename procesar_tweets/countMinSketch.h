#ifndef countMinSketch_H
#define countMinSketch_H

#include <stdio.h>

struct countMinSketch;

typedef struct countMinSketch countMinSketch_t; 

countMinSketch_t* countMinSketch_crear(size_t tamanio);

/* Guarda un elemento en Count-Min Sketch.
*/
void countMinSketch_sumar(countMinSketch_t* countMinSketch, const char *clave);

/* Retorna una aproximacion al numero de veces que la clave ha sido guardada
*/
unsigned int countMinSketch_obtener(countMinSketch_t* countMinSketch, const char *clave);

void countMinSketch_destruir(countMinSketch_t* countMinSketch);

#endif // countMinSketch_H