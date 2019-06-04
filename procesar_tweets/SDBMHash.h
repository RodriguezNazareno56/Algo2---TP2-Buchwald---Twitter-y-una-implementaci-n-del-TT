#ifndef SDBM_H
#define SDBM_H

#include <stdio.h>

/* Aplica sdbm a la clave pasada por parametro
+  Pre: recibe la capacidad del hash y el valor de la clave a aplicar la funcion de hashing.
*  Post: retorna un valor entero asociado a la clave y de rango 0 a capacidad_hash.
*/ 
unsigned int funcion_hash_sdbm(size_t capacidad_hash, const char* clave);

#endif // SDBM_H
