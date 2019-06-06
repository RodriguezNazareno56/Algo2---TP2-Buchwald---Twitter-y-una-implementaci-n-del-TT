#include "split.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Retorna la cantidad de separadores que se encuentran en str.
size_t sepadador_hallar_cantidad(const char *str,const char sep){
	size_t longitud = strlen(str);
	size_t cantidad_sepadores = 0;
	for(int i=0; i < longitud; i++){
		if(str[i] == sep) cantidad_sepadores++;
	}
	return cantidad_sepadores;
}

// Busca las posiciones en donde se encuentran los separadores y añade las posiciones
// en el array posc_separadores[].
void separadores_hallar_posiciones(const char *str, char sep, size_t posc_separadores[]){
	size_t longitud = strlen(str);
	size_t iter_axu = 0;
	for(size_t i=0; i < longitud; i++){
		if(str[i] == sep){
			posc_separadores[iter_axu]=i;
			iter_axu++;
		}
	}
}

char **split(const char *str, char sep){
	size_t cant_sepadores = sepadador_hallar_cantidad(str, sep);
	size_t posc_separadores[cant_sepadores];
	separadores_hallar_posiciones(str, sep, posc_separadores);

	size_t cant_palabras = cant_sepadores + 1;
	char **strv = calloc(cant_palabras+1, sizeof(char*));

	size_t inicio = 0;
	size_t fin;
	for(int i=0; i <= cant_sepadores; i++){
		if(i<cant_sepadores){ 
			// El proximo separador marca el fin de la palabra actual
			fin = posc_separadores[i];
		}else{ 
			// Es la ultima palabra a agregar, no tenemos un separador que marque donde termina
			fin = strlen(str);
		}
		
		size_t long_subcadena = fin - inicio;
		size_t tam_subcadena = long_subcadena + 1; //Espacio para el '\n'	
		char *subcadena = calloc(tam_subcadena,sizeof(char));

		strncpy(subcadena, str + inicio, long_subcadena);
		subcadena[long_subcadena] = '\0'; //Al final de la subcadena
		
		strv[i] = subcadena;
		inicio = fin+1;
		
	}
	strv[cant_palabras] = NULL;
	return strv;
}

void free_arreglo_split(char* strv[]){
	size_t pos=0;
	while( strv[pos] != NULL ){
		free( strv[pos] );
		pos++;
	}
	free(strv);
}