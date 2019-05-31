#include <stdio.h>

#include "crc32bHash.h"
#include "djb2Hash.h"
#include "SDBMHash.h"

#define TAM_VECTOR 30011

unsigned int vector_crc32[TAM_VECTOR] = {0};
unsigned int vector_djb2[TAM_VECTOR] = {0};
unsigned int vector_sdbm[TAM_VECTOR] = {0};


/////////////////// A U X I L I A R E S ////////////////////////////

void vector_crc32_guardar(const char *clave){
	unsigned int posicion = funcion_hash_crc32b(TAM_VECTOR, clave);
	vector_crc32[posicion]++;
}

void vector_djb2_guardar(const char *clave){
	unsigned int posicion = funcion_hash_djb2(TAM_VECTOR, clave);
	vector_djb2[posicion]++;
}

void vector_sdbm_guardar(const char *clave){
	unsigned int posicion = funcion_hash_sdbm(TAM_VECTOR, clave);
	vector_sdbm[posicion]++;
}


unsigned int vector_crc32_obtener(const char *clave){
	unsigned int posicion = funcion_hash_crc32b(TAM_VECTOR, clave);
	return vector_crc32[posicion];
}

unsigned int vector_djb2_obtener(const char *clave){
	unsigned int posicion = funcion_hash_djb2(TAM_VECTOR, clave);
	return vector_djb2[posicion];
}

unsigned int vector_sdbm_obtener(const char *clave){
	unsigned int posicion = funcion_hash_sdbm(TAM_VECTOR, clave);
	return vector_sdbm[posicion];
}


void countMinSketch_guardar(const char *clave){
	vector_crc32_guardar(clave);
	vector_djb2_guardar(clave);
	vector_sdbm_guardar(clave);
}


unsigned int countMinSketch_obtener(const char *clave){
	unsigned int valor_crc32 = vector_crc32_obtener(clave);
	unsigned int valor_djb2 = vector_crc32_obtener(clave);
	unsigned int valor_sdbm = vector_crc32_obtener(clave);
	unsigned int valor_minimo = valor_crc32;
	if(valor_djb2 < valor_minimo){
		valor_minimo = valor_djb2;
	}
	if(valor_sdbm < valor_minimo){
		valor_minimo = valor_sdbm;
	}
	return valor_minimo;
}

