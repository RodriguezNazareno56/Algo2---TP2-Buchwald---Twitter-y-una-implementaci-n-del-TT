#include <stdio.h>
#include <string.h>

#include "split.h"
#include "countMinSketch.h"
#include "vector.h"
#include "heap.h"
#include "trending_topic.h"
#include "hash.h"

char **split_linea_dinamica(vector_t* linea){	
	size_t largo = vector_cantidad(linea);
	char linea_estatica[largo];
	for(size_t i=0; i < largo; i++){
		linea_estatica[i] = (char)linea->datos[i];
	}
	linea_estatica[largo] = '\0';
	
	char** strv;
	strv = split(linea_estatica,',');
	return strv;
}

void cargar_split_en_countMinSketch(char** strv, countMinSketch_t* countMinSketch, trending_topic_t* trending_topic){
	int i = 1;
	while(strv[i] != NULL){
		countMinSketch_sumar(countMinSketch ,(const char*)strv[i]);
		trending_topic_agregar((char*)strv[i], trending_topic, countMinSketch);
		i++;
	}
}

vector_t* leer_linea_stdin(FILE* archivo, int caracter){
	size_t tam_linea = 10;
	vector_t* linea = vector_crear(tam_linea);
	if(!linea) return NULL;
	vector_guardar(linea, 0, caracter);
	size_t posc_linea = 1;
	while((caracter = fgetc(archivo)) != '\n'){
		if(!vector_guardar(linea, posc_linea, caracter)){
			tam_linea*=2;
			vector_redimensionar(linea, tam_linea);
			vector_guardar(linea, posc_linea, caracter);
		}
		posc_linea++;
	}
	vector_guardar(linea, posc_linea, '\0');
	return linea;
}


int main(int argc, char *argv[]){
	if(argc > 3 || argc == 1){ 
		fprintf(stderr,"Error: Cantidad erronea de parametros\n");
		return -1;
	}

	size_t n_lineas = atoi(argv[1]);
	size_t k_tt = atoi(argv[2]);

	countMinSketch_t* countMinSketch = countMinSketch_crear(30011);
	if(!countMinSketch) return -1;

	FILE* archivo = stdin;
	int caracter;
	size_t ronda_numero = 0;
	while((caracter = fgetc(archivo)) != EOF){

		trending_topic_t* trending_topic = trending_topic_crear(k_tt, countMinSketch);
		if(!trending_topic) return -1;
		ronda_numero++;

		for(int i = 0; i < n_lineas && caracter!= EOF; i++){
			// Cargo la linea en un vector dinamico
			vector_t* linea = leer_linea_stdin(archivo, caracter);
			// 	Le realizo split a la linea cargada en el vector dinamico
			char** linea_split;
			linea_split = split_linea_dinamica(linea);	
			vector_destruir(linea);	
			//	Guardo todos los elementos recibidos en la linea, menos el usuario
			cargar_split_en_countMinSketch(linea_split,countMinSketch, trending_topic);

			free_arreglo_split(linea_split);
			caracter = fgetc(archivo);
		}
		trending_topic_imprimir(trending_topic, ronda_numero);
		trending_topic_destruir(trending_topic);
	}
	fclose(archivo); 
	countMinSketch_destruir(countMinSketch);

	
	return 0;
	
}
