#ifndef CRC32B_H
#define CRC32B_H

#include <stdio.h>

/* Aplica crc32b a la clave pasada por parametro
+  Pre: recibe la capacidad del hash y el valor de la clave a aplicar la funcion de hashing.
*  Post: retorna un valor entero asociado a la clave y de rango 0 a capacidad_hash.
*/ 
unsigned int funcion_hash_crc32b(size_t capacidad_hash, const char* clave);

#endif // CRC32B_H