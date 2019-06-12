#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hash.h"
#include "lista.h"
#include "split.h"
#include "vector.h"


char **split_linea_dinamica(vector_t* linea){   
    size_t largo = vector_cantidad(linea);
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

char* copiar_string_char(const char* clave){
    size_t clave_longitud = strlen(clave);
    char* clave_copia = malloc(sizeof(char) * (clave_longitud+1));
    if(!clave_copia) return NULL;
    strcpy(clave_copia, clave);
    clave_copia[clave_longitud] = '\0';
    return clave_copia;
}

bool hashtag_usado(lista_t* hashtags_utilizados, const char* hashtag){
    lista_iter_t* iter = lista_iter_crear(hashtags_utilizados);
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

void funcion_destruir_hash(void* dato){
    lista_t* lista = (lista_t*)dato;
    lista_destruir(lista,free);
}

hash_t* guardar_usuarios_con_hashtags(FILE* archivo){
    hash_t* hash_user_hashtags = hash_crear(funcion_destruir_hash);
    if(!hash_user_hashtags) return NULL;
    int caracter;
    while ((caracter = fgetc(archivo)) != EOF){
        vector_t* linea = leer_linea_stdin(archivo, caracter);  
        char** linea_spliteada = split_linea_dinamica(linea);
        const char* usuario = linea_spliteada[0];

        // Lista con hashtags utilizados por el usuario
        if(!hash_pertenece(hash_user_hashtags,usuario)){
            lista_t* lista = lista_crear();
            if(!lista) return NULL;
            hash_guardar(hash_user_hashtags, usuario, lista);
        }
        lista_t* lista_hashtags_utilizados;
        lista_hashtags_utilizados = (lista_t*)hash_obtener(hash_user_hashtags, usuario);

        // Guardo los hashtags utilizados por el usuario sin repeticiones
        int i = 1;
        while(linea_spliteada[i] != NULL){
            if(!hashtag_usado(lista_hashtags_utilizados, linea_spliteada[i])){
                char* hashtag_copia = copiar_string_char(linea_spliteada[i]);
                lista_insertar_ultimo(lista_hashtags_utilizados, hashtag_copia);
            }
            i++;
        }

        free_arreglo_split(linea_spliteada);
    }
    return hash_user_hashtags;
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


hash_t* guardar_usuarios_cantidad_hashtag(hash_t* hash_user_hashtags, int* valor_maximo){
    int valor_maximo_aux = 1;
    hash_t* hash_cantidad_hashtags = hash_crear(funcion_destruir_hash);
    if(!hash_cantidad_hashtags) return NULL;
    hash_iter_t* hash_user_hashtags_iter = hash_iter_crear(hash_user_hashtags);
    if(!hash_user_hashtags_iter) return NULL;
    while(!hash_iter_al_final(hash_user_hashtags_iter)){
        const char* usuario = hash_iter_ver_actual(hash_user_hashtags_iter);
        int cantidad_hashtags_usuario = contar_hashtags_por_usuario(hash_user_hashtags, usuario);
        char cantidad_hashtags_usuario_char[10];
        sprintf(cantidad_hashtags_usuario_char,"%d",cantidad_hashtags_usuario);

        if (cantidad_hashtags_usuario == -1) return NULL;
        if (cantidad_hashtags_usuario > valor_maximo_aux){
            valor_maximo_aux = cantidad_hashtags_usuario;
        }
        // Lista de usuarios con igual cantidad de hashtags
        if (!hash_pertenece(hash_cantidad_hashtags, cantidad_hashtags_usuario_char)){
            lista_t* lista = lista_crear();
            hash_guardar(hash_cantidad_hashtags, cantidad_hashtags_usuario_char, lista);
        }
        lista_t* usuarios = (lista_t*)hash_obtener(hash_cantidad_hashtags, cantidad_hashtags_usuario_char);
        char* usuario_copia = copiar_string_char(usuario);
        if (lista_esta_vacia(usuarios)){
            lista_insertar_primero(usuarios, usuario_copia);
        }
        else{
            lista_iter_t* lista_iter = lista_iter_crear(usuarios);
            char* actual = (char*)(lista_iter_ver_actual(lista_iter));
            if (strcmp(actual, usuario_copia) < 0){
                while(actual!=NULL && strcmp(actual, usuario_copia) < 0){
                    lista_iter_avanzar(lista_iter);
                    actual = (char*)lista_iter_ver_actual(lista_iter);
                }
            }
            lista_iter_insertar(lista_iter, usuario_copia);
            lista_iter_destruir(lista_iter);
        }
        hash_iter_avanzar(hash_user_hashtags_iter);
    }
    hash_iter_destruir(hash_user_hashtags_iter);
    *valor_maximo = valor_maximo_aux;

    return hash_cantidad_hashtags;
}


void imprimir_ordenado(hash_t* hash_cantidad_hashtags, int valor_maximo){
    bool es_el_primero = true;
    for (int i = 1; i <= valor_maximo; i++){
        char i_char[12];
        sprintf(i_char,"%d",i); 
		if (hash_pertenece(hash_cantidad_hashtags, i_char)){
            printf("%i: ", i);
            lista_t* usuarios = (lista_t*)(hash_obtener(hash_cantidad_hashtags, i_char));
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
            es_el_primero = true;
            lista_iter_destruir(iter_lista);
            printf("\n");
        }
    }
}


int main(int argc, char *argv[]){
    if(argc != 2){ 
        fprintf(stderr,"Error: Cantidad erronea de parametros\n");
        return -1;
    }
    FILE* archivo = fopen(argv[1], "rt");
    if(!archivo){
        fprintf(stderr,"Error: archivo fuente inaccesible\n");
        return -1;
    }

    hash_t* hash_user_hashtags = guardar_usuarios_con_hashtags(archivo);
    if(!hash_user_hashtags) return -1;
    fclose(archivo);

    int valor_maximo;
    hash_t* hash_cantidad_hashtags = guardar_usuarios_cantidad_hashtag(hash_user_hashtags, &valor_maximo);
    if(!hash_cantidad_hashtags) return -1;
    hash_destruir(hash_user_hashtags);

    imprimir_ordenado(hash_cantidad_hashtags, valor_maximo);
    hash_destruir(hash_cantidad_hashtags);

    return 0;
}
