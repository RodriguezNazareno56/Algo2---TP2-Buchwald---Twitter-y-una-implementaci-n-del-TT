#include <stdio.h>

unsigned int djb2(const char *str) {
        unsigned int hash = 5381;
        int c;
        while ((c = *str++))
                hash = hash * 33 ^ c;
        return hash;
}

unsigned int funcion_hash_djb2(size_t capacidad_hash, char* clave){
	unsigned int  codominio;
	codominio = djb2(clave);

	return(codominio % (unsigned int)capacidad_hash);
}
