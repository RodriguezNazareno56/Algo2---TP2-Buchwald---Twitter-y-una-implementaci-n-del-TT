#include <stdio.h>
#include <string.h>

#include "split.h"
#include "countMinSketch.h"
#include "vector.h"
#include "heap.h"
#include "hash.h"

typedef struct hashtag{
    char* hashtag_char;
    countMinSketch_t* countMinSketch;
}hashtag_t;


typedef struct trending_topic{
	heap_t* heap_de_menores_tt;
	hash_t* hash_tt;
	size_t cantidad;
}trending_topic_t;


char* copiar_hashtag_char(const char* clave){
	size_t clave_longitud = strlen(clave);
	char* clave_copia = malloc(sizeof(char) * (clave_longitud+1));
	if(!clave_copia) return NULL;
	strcpy(clave_copia, clave);
	return clave_copia;
}

hashtag_t* hashtag_crear(const char *hashtag_char, countMinSketch_t* countMinSketch){
	hashtag_t* hashtag = malloc(sizeof(hashtag_t));
	if(!hashtag) return NULL;
	if(!hashtag_char){
		hashtag->hashtag_char = NULL;
		hashtag->countMinSketch = countMinSketch;
		return hashtag;
	}
	char* hashtag_char_copia = copiar_hashtag_char(hashtag_char);
	hashtag->hashtag_char = hashtag_char_copia;
	hashtag->countMinSketch = countMinSketch;
	return hashtag;
}

unsigned int hashtag_repeticiones(const hashtag_t* hashtag){
	if(!hashtag) return 0;
	return countMinSketch_obtener(hashtag->countMinSketch,hashtag->hashtag_char);
}

void hashtag_destruir(void* hashtag){
	free(((hashtag_t*)hashtag)->hashtag_char);
	free((hashtag_t*)hashtag);
}

int hashtag_cmp(const void *a, const void *b){
	const hashtag_t* hashtag_1 = a;
	const hashtag_t* hashtag_2 = b;
	if(hashtag_1->hashtag_char == NULL && hashtag_2->hashtag_char == NULL) return 0;
	if(hashtag_1->hashtag_char == NULL) return 1;
	if(hashtag_2->hashtag_char == NULL) return -1;
	if (hashtag_repeticiones(hashtag_1) > hashtag_repeticiones(hashtag_2))
        return -1;
    if (hashtag_repeticiones(hashtag_1) < hashtag_repeticiones(hashtag_2))
    	return 1;
    return strcmp(hashtag_1->hashtag_char,hashtag_2->hashtag_char);
}



trending_topic_t* trending_topic_crear(size_t cantidad, countMinSketch_t* countMinSketch){
	trending_topic_t* trending_topic = malloc(sizeof(trending_topic_t));
	if(!trending_topic) return NULL;
	trending_topic->cantidad = cantidad;

	heap_t* heap = heap_crear(hashtag_cmp);
	if(!heap) return NULL;
	for(size_t i=0; i < cantidad; i++){
		hashtag_t* hashtag = hashtag_crear(NULL, countMinSketch);
		if(!hashtag) return NULL;
		heap_encolar(heap, hashtag);
	}
	trending_topic->heap_de_menores_tt = heap;

	hash_t* hash = hash_crear(free);
	if(!hash) return NULL;
	trending_topic->hash_tt = hash;
	return trending_topic;
}

void trending_topic_destruir(trending_topic_t* trending_topic){
	hash_destruir(trending_topic->hash_tt);
	heap_destruir(trending_topic->heap_de_menores_tt, hashtag_destruir);
	free(trending_topic);
}

bool trending_topic_agregar(const char *hashtag_actual, trending_topic_t* trending_topic, countMinSketch_t* countMinSketch){
	const char* tt_menor = ((hashtag_t*)heap_ver_tope(trending_topic->heap_de_menores_tt))->hashtag_char;
	size_t repeticiones_tt_menor = 0;
	if(tt_menor != NULL){
		repeticiones_tt_menor = countMinSketch_obtener(countMinSketch, tt_menor);
	}
	size_t repeticiones_hashtag_actual = countMinSketch_obtener(countMinSketch, hashtag_actual);

	if(!hash_pertenece(trending_topic->hash_tt, hashtag_actual)){
		if(repeticiones_hashtag_actual > repeticiones_tt_menor ||
			(repeticiones_hashtag_actual == repeticiones_tt_menor && strcmp(hashtag_actual,tt_menor) < 0)){
			hashtag_t* hashtag_nuevo = hashtag_crear(hashtag_actual, countMinSketch);
			hashtag_t* hashtag_destronado = heap_desencolar(trending_topic->heap_de_menores_tt);
			if(hashtag_destronado->hashtag_char != NULL){
				hash_borrar(trending_topic->hash_tt, hashtag_destronado->hashtag_char);
			}
			hashtag_destruir(hashtag_destronado);

			heap_encolar(trending_topic->heap_de_menores_tt, hashtag_nuevo);
			hash_guardar(trending_topic->hash_tt, hashtag_nuevo->hashtag_char, NULL);
			return true;
		}
		else{
			heap_actualizar(trending_topic->heap_de_menores_tt);
			return true;
		}
	}
	return false;
}

void trending_topic_imprimir(trending_topic_t* trending_topic, size_t ronda_numero){
	if(heap_esta_vacio(trending_topic->heap_de_menores_tt)){
		printf("--- %ld\n", ronda_numero);
		return;
	} 
	hashtag_t* hashtag_actual = (hashtag_t*)heap_desencolar(trending_topic->heap_de_menores_tt);
	trending_topic_imprimir(trending_topic, ronda_numero);
	if(hashtag_actual->hashtag_char){
		printf("%d %s \n",countMinSketch_obtener(hashtag_actual->countMinSketch,hashtag_actual->hashtag_char),hashtag_actual->hashtag_char);
	}
	heap_encolar(trending_topic->heap_de_menores_tt, hashtag_actual);
}