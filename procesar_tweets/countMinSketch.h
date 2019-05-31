#ifndef countMinSketch_H
#define countMinSketch_H

#include <stdio.h>

/* Guarda un elemento en Count-Min Sketch.
*/
void countMinSketch_guardar(const char *clave);

/* Retorna una aproximacion al numero de veces que la clave ha sido guardada
*/
unsigned int countMinSketch_obtener(const char *clave);

#endif // countMinSketch_H