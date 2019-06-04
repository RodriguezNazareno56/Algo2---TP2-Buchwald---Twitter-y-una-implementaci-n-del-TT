#ifndef DJB2_H
#define DJB2_H

#include <stdio.h>

/* Aplica djb2 a la clave pasada por parametro
+  Pre: recibe la capacidad del hash y el valor de la clave a aplicar la funcion de hashing.
*  Post: retorna un valor entero asociado a la clave y de rango 0 a capacidad_hash.
*/ 
unsigned int funcion_hash_djb2(size_t capacidad_hash, const char* clave);

#endif // DJB2_H