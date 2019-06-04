#ifndef countMinSketch_H
#define countMinSketch_H

#include <stdio.h>

struct countMinSketch;

typedef struct countMinSketch countMinSketch_t; 

/* Crea el countMinSketch
*/ 
countMinSketch_t* countMinSketch_crear(size_t tamanio);

/* Suma el elemento a countMInSketch
*/ 
void countMinSketch_sumar(countMinSketch_t* countMinSketch, const char *clave);

/* Retorna una aproximacion al numero de veces que la clave ha sido sumada.
*/
unsigned int countMinSketch_obtener(countMinSketch_t* countMinSketch, const char *clave);

/* Destruye el countMinSketch
*/
void countMinSketch_destruir(countMinSketch_t* countMinSketch);

#endif // countMinSketch_H