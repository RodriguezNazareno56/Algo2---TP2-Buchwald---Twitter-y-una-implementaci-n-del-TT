#include "countMinSketch.h"
#include <stdio.h>
#include <stdlib.h>
#include "crc32bHash.h"
#include "djb2Hash.h"
#include "SDBMHash.h"



struct countMinSketch{
	size_t tamanio;
	unsigned int* vector_crc32;
	unsigned int* vector_djb2;
	unsigned int* vector_sdbm;
};

/// A U X I L I A R E S

void vector_crc32_sumar(unsigned int* vector_crc32, size_t tamanio ,const char *clave){
	unsigned int posicion = funcion_hash_crc32b(tamanio, clave);
	vector_crc32[posicion]++;
}

void vector_djb2_sumar(unsigned int* vector_djb2, size_t tamanio ,const char *clave){
	unsigned int posicion = funcion_hash_djb2(tamanio, clave);
	vector_djb2[posicion]++;
}

void vector_sdbm_sumar(unsigned int* vector_sdbm, size_t tamanio ,const char *clave){
	unsigned int posicion = funcion_hash_sdbm(tamanio, clave);
	vector_sdbm[posicion]++;
}

unsigned int vector_crc32_obtener(unsigned int* vector_crc32, size_t tamanio ,const char *clave){
	unsigned int posicion = funcion_hash_crc32b(tamanio, clave);
	return vector_crc32[posicion];
}

unsigned int vector_djb2_obtener(unsigned int* vector_djb2, size_t tamanio ,const char *clave){
	unsigned int posicion = funcion_hash_djb2(tamanio, clave);
	return vector_djb2[posicion];
}

unsigned int vector_sdbm_obtener(unsigned int* vector_sdbm, size_t tamanio ,const char *clave){
	unsigned int posicion = funcion_hash_sdbm(tamanio, clave);
	return vector_sdbm[posicion];
}

////////////////////////////////////////////////////////////////

void setear_a_cero(unsigned int* vector, size_t tamanio){
	for(size_t i=0; i<tamanio; i++){
		vector[i] = 0;
	}
}

countMinSketch_t* countMinSketch_crear(size_t tamanio){
	countMinSketch_t* countMinSketch = malloc(sizeof(countMinSketch_t));
	if(!countMinSketch) return NULL;
	unsigned int* vector_crc32 = calloc(tamanio, sizeof(int));;
	if(!vector_crc32) return NULL;
	setear_a_cero(vector_crc32, tamanio);
	unsigned int* vector_djb2 = calloc(tamanio, sizeof(int));
	if(!vector_djb2){
		free(vector_crc32);
		return NULL;
	} 
	setear_a_cero(vector_djb2, tamanio);
	unsigned int* vector_sdbm = calloc(tamanio, sizeof(int));
	if(!vector_sdbm){
		free(vector_crc32);
		free(vector_djb2);
		return NULL;
	}
	setear_a_cero(vector_sdbm, tamanio);
	countMinSketch->vector_crc32 = vector_crc32;
	countMinSketch->vector_djb2 = vector_djb2;
	countMinSketch->vector_sdbm = vector_sdbm;
	countMinSketch->tamanio = tamanio;
	return countMinSketch;
}

void countMinSketch_sumar(countMinSketch_t* countMinSketch, const char *clave){
	vector_crc32_sumar(countMinSketch->vector_crc32, countMinSketch->tamanio, clave);
	vector_djb2_sumar(countMinSketch->vector_djb2, countMinSketch->tamanio, clave);
	vector_sdbm_sumar(countMinSketch->vector_sdbm, countMinSketch->tamanio, clave);
}

unsigned int countMinSketch_obtener(countMinSketch_t* countMinSketch, const char *clave){
	unsigned int valor_crc32 = vector_crc32_obtener(countMinSketch->vector_crc32, countMinSketch->tamanio, clave);
	unsigned int valor_djb2 = vector_djb2_obtener(countMinSketch->vector_djb2, countMinSketch->tamanio, clave);
	unsigned int valor_sdbm = vector_sdbm_obtener(countMinSketch->vector_sdbm, countMinSketch->tamanio, clave);
	unsigned int valor_minimo = valor_crc32;
	if(valor_djb2 < valor_minimo){
		valor_minimo = valor_djb2;
	}
	if(valor_sdbm < valor_minimo){
		valor_minimo = valor_sdbm;
	}
	return valor_minimo;
}
