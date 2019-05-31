#include <stdio.h>
#include <string.h>

unsigned int sdbmHash(const char* str, size_t length) {
	unsigned int hash = 0;
	size_t i = 0;

	for (i = 0; i < length; str++, i++){
		hash = (*str) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

unsigned int funcion_hash_sdbm(size_t capacidad_hash, const char* clave){
	unsigned int codominio;
	size_t longitud = strlen(clave);
	codominio = sdbmHash(clave, longitud);

	return(codominio % (unsigned int)capacidad_hash);
}