#include <stdio.h>

unsigned int crc32b(const char *message) {
   int i, j;
   unsigned int byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
   while (message[i] != 0) {
      byte = message[i];            // Get next byte.
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) {    // Do eight times.
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i = i + 1;
   }
   return ~crc;
}

unsigned int funcion_hash_crc32b(size_t capacidad_hash, char* clave){
	unsigned int codominio;
	codominio = crc32b(clave);

	return(codominio % (unsigned int)capacidad_hash);
}