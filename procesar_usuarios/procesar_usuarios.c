#include <stdio.h>
#include "vector.c"
#include "hash.c"
#include "lista.c"
#include <stdbool.h>
#include <string.h>
#include "strutil.c"

#define VALOR_MAXIMO "vmax"

char **split_linea_dinamica(vector_t* linea){	
	int largo = vector_cantidad(linea);
	char linea_estatica[largo];
	for(size_t i=0; i < largo; i++){
		linea_estatica[i] = (char)linea->datos[i];
	}
	linea_estatica[largo] = '\0';
	
	char** strv;
	strv = split(linea_estatica,',');
    vector_destruir(linea);
	return strv;
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

bool hashtag_usado(lista_t* datos, char* hashtag){
    lista_iter_t* iter = lista_iter_crear(datos);
    if (!iter) return false;
    while(!lista_iter_al_final(iter)){
        if (strcmp((char*)(lista_iter_ver_actual(iter)), hashtag) == 0){
            lista_iter_destruir(iter);
            return true;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return false;
}

hash_t* guardar_usuarios_con_hashtags(FILE* archivo){
    hash_t* hash = hash_crear(NULL);
    if (!hash) return NULL;
    int caracter = fgetc(archivo);
    while (caracter != EOF){
        vector_t* linea = leer_linea_stdin(archivo, caracter);
        char** linea_spliteada = split_linea_dinamica(linea);
        caracter = fgetc(archivo); // Esto tomaría el '\n'o el EOF.
        const char* usuario = linea_spliteada[0];
        lista_t* datos;
        if (!hash_pertenece(hash, usuario)){
            datos = lista_crear();
            if (!datos) return NULL;
        }
        else{
            datos = (lista_t*)(hash_obtener(hash, usuario));
        }
        for (int i = 1; *linea_spliteada[i] != '\0'; i++){
            if (hashtag_usado(datos, linea_spliteada[i])){
                continue;
            }
            else{
                lista_insertar_ultimo(datos, (void*)(linea_spliteada[i]));
            }
        }
        if (!hash_pertenece(hash, usuario)){
            hash_guardar(hash, usuario, (void*)(datos));
        }
        if (caracter != EOF){
            caracter = fgetc(archivo);
        }
        free_strv(linea_spliteada);
    }
    return hash;
}

int contar_hashtags_por_usuario(hash_t* hash, const char* usuario){
    lista_t* datos = (lista_t*)(hash_obtener(hash, usuario));
    int cantidad_hashtags = 0;
    lista_iter_t* iter = lista_iter_crear(datos);
    if (!iter) return -1;
    while(!lista_iter_al_final(iter)){
        cantidad_hashtags++;
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return cantidad_hashtags;
}

hash_t* guardar_usuarios_cantidad_hashtag(hash_t* hashtags){
    char* aux = VALOR_MAXIMO;
    int valor_maximo = 1;
    hash_t* hash_cantidad_hashtags = hash_crear(NULL);
    if (!hash_cantidad_hashtags) return NULL;
    hash_iter_t* hash_iter = hash_iter_crear(hashtags);
    if (!hash_iter) return NULL;
    while(!hash_iter_al_final(hash_iter)){
        const char* usuario = hash_iter_ver_actual(hash_iter);
        int cantidad_hashtags_usuario = contar_hashtags_por_usuario(hashtags, usuario);
        if (cantidad_hashtags_usuario == -1) return NULL;
        if (cantidad_hashtags_usuario > valor_maximo){
            valor_maximo = cantidad_hashtags_usuario;
        }
        if (!hash_pertenece(hash_cantidad_hashtags, (char*)(cantidad_hashtags_usuario))){
            lista_t* usuarios = lista_crear();
            lista_insertar_ultimo(usuarios, (void*)(usuario));
            hash_guardar(hash_cantidad_hashtags, (char*)(cantidad_hashtags_usuario), (void*)(usuarios));
        }
        else{
            lista_t* usuarios = (lista_t*)(hash_obtener(hash_cantidad_hashtags, (char*)(cantidad_hashtags_usuario)));
            lista_insertar_ultimo(usuarios, (void*)(usuario));
        }
        hash_iter_avanzar(hash_iter);
    }
    hash_guardar(hash_cantidad_hashtags, aux, (void*)(valor_maximo));
    hash_iter_destruir(hash_iter);
    return hash_cantidad_hashtags;
}

void imprimir_ordenado(hash_t* hash_cantidad_hashtags){
    int valor_maximo = (int)(hash_obtener(hash_cantidad_hashtags, VALOR_MAXIMO));
    bool es_el_primero = true;
    for (int i = valor_maximo; i > 0; i--){
        if (hash_pertenece(hash_cantidad_hashtags, (char*)(i))){
            printf("%i: ", i);
            lista_t* usuarios = (lista_t*)(hash_obtener(hash_cantidad_hashtags, (char*)(i)));
            lista_iter_t* iter_lista = lista_iter_crear(usuarios);
            if (!iter_lista) return;
            while (!lista_iter_al_final(iter_lista)){
                char* usuario = (char*)(lista_iter_ver_actual(iter_lista));
                if (es_el_primero){
                    printf("%s", usuario);
                    es_el_primero = false;
                }
                else{    
                    printf(", %s", usuario);
                }
                lista_iter_avanzar(iter_lista);
            }
            lista_iter_destruir(iter_lista);
            printf("\n");
        }
    }
}

int main(int argc, char* argv[]){
    FILE* archivo = fopen(argv[1], "r");
	if(!archivo){
		fprintf(stderr,"Error: archivo fuente inaccesible\n");
		return -1;
	}
    if (argc != 2){
		fprintf(stderr, "Error: Cantidad erronea de parametros\n");
        return -1;
    }
    hash_t* hashtags = guardar_usuarios_con_hashtags(archivo);
    if (!hashtags) return -1;
    hash_t* hash_cantidad_hashtags = guardar_usuarios_cantidad_hashtag(hashtags);
    if (hash_cantidad_hashtags) return -1;
    imprimir_ordenado(hash_cantidad_hashtags);
    hash_iter_t* iter_hashtags = hash_iter_crear(hashtags);
    hash_iter_t* iter_hash_cantidad = hash_iter_crear(hash_cantidad_hashtags);
    while(!hash_iter_al_final(iter_hashtags)){
        const char* actual = hash_iter_ver_actual(iter_hashtags);
        lista_t* datos = (lista_t*)(hash_obtener(hashtags, actual));
        lista_destruir(datos, NULL);
    }
    hash_iter_destruir(iter_hashtags);
    hash_destruir(hashtags);
    while(!hash_iter_al_final(iter_hash_cantidad)){
        const char* actual = hash_iter_ver_actual(iter_hash_cantidad);
        lista_t* datos = (lista_t*)(hash_obtener(hash_cantidad_hashtags, actual));
        lista_destruir(datos, NULL);
    }
    hash_iter_destruir(iter_hash_cantidad);
    hash_destruir(hash_cantidad_hashtags);
    fclose(archivo);
    return 0;
}