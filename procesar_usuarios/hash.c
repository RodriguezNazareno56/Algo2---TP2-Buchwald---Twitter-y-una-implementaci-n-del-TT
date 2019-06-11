#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CAPACIDAD_INICIAL 29

#define CARGA_MAXIMA_LISTA 3 // Porcentual
#define CARGA_MINIMA_LISTA 0.3 // Porcentual

#define FACTOR_REDIMENSIONAR_AGRANDAR 2
#define FACTOR_REDIMENSIONAR_ACHICAR 2

// Array de numeros primos hasta 800 millones, en orden acendente N aproximadamente duplica
// el tamaño de N-1. De acuerdo a un factor de redimensionamiento igual a 2. 
// Superada la cifra de los 800 millones de posiciones en tabla. Se redimensionara con el factor
// de carga.
#define CANTIDAD_ELEMENTOS_ARREGLO_PRIMOS 26
const size_t NUMEROS_PRIMOS[CANTIDAD_ELEMENTOS_ARREGLO_PRIMOS] = {CAPACIDAD_INICIAL, 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
    24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917,
    25165843, 50331653, 100663319, 201326611, 402653189, 805306457};//26 elementos

// Importante! De cambiarse la CAPACIDAD_INICIAL, se debera adecuar el array de NUMEROS_PRIMOS
// de modo tal que se lo converse ordenado ascendentemente.
// De buscar alterar el factor de redimension, tambien se debera adecuar el array de NUMEROS_PRIMOS
// a la finalidad que en este se encuentren numeros de modo tal {n, aproximadamente(n*factor_redimension)(primo)}.


////////////////////////  E S T R U C T U R A   H A S H  //////////////////////////////

typedef struct nodo_hash{
	char* clave;
	void* dato;
}nodo_hash_t;

struct hash{
	size_t cantidad;
	size_t capacidad_tabla; 
	int array_num_primos_posc_actual;
	hash_destruir_dato_t funcion_destruir_dato;
	lista_t** tabla;
};


////////////////////////  A U X I L I A R E S   H A S H  //////////////////////////////
////////////////////////    D O C U M E N T A C I O N    //////////////////////////////

// Crea un nodo incializandolo con los valores pasados por parametro. Para la clave
// se vale de la funcion "nodo_hash_guardar_clave" para guarda una copia.
// Pre: -
// Post: Retorna el nodo en caso de crear exitosamente al mismo, NULL en caso contrario.
nodo_hash_t *nodo_hash_crear(const char *clave, void *dato);

// Genera una copia de la clave pasada por parametro.
// Pre: -
// Post: Retorna una copia de la clave en caso de exito al copiarla, NULL en caso contrario.
char* nodo_hash_guardar_clave(const char* clave);

// Destruye el nodo_hash, liberando la memoria de todos sus miembros
// Pre: Se le otorga un funcion_destruir_dato para liberar la memoria del dato del nodo o 
// NULL en caso de no ser necesaria.
// Post: Libera la memoria correspondiente al hash, la clave y al dato en caso de recibir 
// funcion de destruiccion
void nodo_hash_destruir(nodo_hash_t* nodo_hash, hash_destruir_dato_t funcion_destruir_dato);

// Setea a NULL todos los punteros a lista_t de la tabla
// Pre: La tabla fue creada y el tamano de la misma inidicado.
// Post: Todas las posiciones de la tabla apuntan a NULL.
void tabla_setear_a_null(lista_t** tabla, size_t tamanio);

// Inicializa la posicion en la tabla, creando una lista enlazada vacia.
// Pre: La estructura hash fue inicializada, la posicion no posee una lista inicializada.
// Post: Retorna true si se genero una lista enlazada para la posicion en la tabla hash,
// false si no ha sido posible o ya existia una lista en dicha posicion.
bool tabla_crear_lista(hash_t *hash, size_t posicion);

// Retorna el nodo correpondiente al elemento buscado,  a traves de su clave,
// en la lista correspiente a una posicion de la tabla. (hash abierto).
// Pre: la lista pasada por parametro corresponda a la lista del hash (abierto) que se 
// encuentra en la posicion que la funcion de hash a indicado que deberia de contener 
// al nodo de acuerdo a su clave. 
// Post: Retorna el nodo en caso de exito, NULL en caso de no encontrarlo o error.
nodo_hash_t *tabla_obtener_nodo_en_lista(lista_t *lista_en_tabla, const char* clave);

// Retorna la posicion en que se encuentra el elemento buscado, a traves de su clave,
// en la lista correspiente a una posicion de la tabla. (hash abierto).
// Pre: la lista pasada por parametro corresponda a la lista del hash (abierto) que se 
// encuentra en la posicion que la funcion de hash a indicado que deberia de contener 
// al nodo de acuerdo a su clave. 
// Post: Retorna la posicion en caso de exito, -1 en caso de no encontrarlo o error.
size_t tabla_obtener_posicion_en_lista(lista_t *lista_en_tabla, const char* clave);

// Elimina el nodo correspondiente a la clave en la lista de la tabla. Retornando
// el nodo en cuestion.
// Pre: la lista pasada por parametro corresponda a la lista del hash (abierto) que se 
// encuentra en la posicion que la funcion de hash a indicado que deberia de contener 
// al nodo de acuerdo a su clave. 
// Post: Elimina el nodo del hash y lo retorna sin destruirlo. Retorna NULL en caso de eror.
nodo_hash_t* tabla_borrar_nodo_en_lista(lista_t *lista_en_tabla, const char* clave);

// Dada una clave asigna un valor de hash haciendo uso de una funcion de hashing.
// Pre: La capacidad del hash a sido bien especificada.
// Post: Retorna un valor entre 0 y capicidad_hash.
size_t funcion_hash(size_t capacidad_hash, const char* clave);

// Dada una clave y su longitud en caracteres retorna un valor numero.
// Pre: -
// Post: Retorna un valor de hash.
size_t SDBMHash(const char* str, size_t length);

// Analiza si es necesario realizar un redimension del hash.
// Pre: -
// Post: Retorna true de resultar necesaria la redimension, false en caso contrario.
bool hash_analizar_necesidad_de_redimension(hash_t *hash);

// Calcula el valor del tamaño que se usara para redimensionar.
// Pre: -
// Post: Retorna el valor correspondiente para redimensionar.
size_t hash_redimensionar_calcular_tam_nuevo(hash_t *hash);

// La tabla de hash es redimensionada con un valor de capacidad_nueva dado por
// la funcion "hash_redimensionar_calcular_tam_nuevo".
// Pre: El hash precisa ser redimensionado.
// Post: Si la redimension de realiza exitosamente retorna true, false en caso contrario.
bool hash_redimensionar(hash_t *hash);

////////////////////////  P R I M I T I V A S   H A S H  //////////////////////////////


hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash) return NULL;
	// Genero un array de punteros a listas
	lista_t** array_de_listas = malloc(sizeof(lista_t*) * CAPACIDAD_INICIAL);
	if(!array_de_listas){
		free(hash);
		return NULL;
	}
	hash->tabla = array_de_listas;
	hash->cantidad = 0;
	hash->capacidad_tabla = CAPACIDAD_INICIAL;
	hash->funcion_destruir_dato = destruir_dato;
	tabla_setear_a_null(hash->tabla, hash->capacidad_tabla);

	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	size_t posc_en_tabla = funcion_hash(hash->capacidad_tabla, clave);
	if(hash_pertenece(hash, clave)){ // Si ya pertene
		nodo_hash_t* nodo_hash = tabla_obtener_nodo_en_lista(hash->tabla[posc_en_tabla], clave);
		if (hash -> funcion_destruir_dato){
			hash -> funcion_destruir_dato(nodo_hash -> dato);
		}
		nodo_hash->dato = dato; // Reemplazo el dato del nodo que contiene la misma clave
		return true;
	}
	if(hash->tabla[posc_en_tabla] == NULL){ //Aun no se ha creado la lista en esta posicion
		if(!tabla_crear_lista(hash, posc_en_tabla)) return false;
	}
	nodo_hash_t* nodo_hash = nodo_hash_crear(clave, dato);
	if(!nodo_hash) return false;
	if(!lista_insertar_ultimo(hash->tabla[posc_en_tabla], nodo_hash)){
		free(nodo_hash);
		return false; // Fallo la insercion.
	}
	hash->cantidad++;

	if(hash_analizar_necesidad_de_redimension(hash)){
		bool redimension = hash_redimensionar(hash);
		return redimension;
	}
	
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	size_t posc_en_tabla = funcion_hash(hash->capacidad_tabla, clave);
	if(hash->tabla[posc_en_tabla] == NULL) return NULL;
	nodo_hash_t* nodo_hash_borrado = tabla_borrar_nodo_en_lista(hash->tabla[posc_en_tabla], clave);
	if(!nodo_hash_borrado) return NULL;
	void* dato_borrado = nodo_hash_borrado->dato;
	free(nodo_hash_borrado->clave);
	free(nodo_hash_borrado); 
	hash->cantidad--;
	
	if(hash_analizar_necesidad_de_redimension(hash)){
		bool redimension = hash_redimensionar(hash);
		if (!redimension) return NULL;
	}
	
	return dato_borrado;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	size_t posc_en_tabla = funcion_hash(hash->capacidad_tabla, clave);
	if(!hash_pertenece(hash, clave)) return NULL;
	nodo_hash_t* nodo_hash_buscado = tabla_obtener_nodo_en_lista(hash->tabla[posc_en_tabla], clave);
	if(!nodo_hash_buscado) return NULL;
	return nodo_hash_buscado->dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t posc_en_tabla = funcion_hash(hash->capacidad_tabla, clave);
	if(hash->tabla[posc_en_tabla] == NULL) return false;
	// Lo busco en la lista de la tabla
	size_t posc_en_lista_tabla = tabla_obtener_posicion_en_lista(hash->tabla[posc_en_tabla], clave);
	if(posc_en_lista_tabla == -1) return false;
	return true;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}

void hash_destruir(hash_t *hash){
	for(int i=0; i < hash->capacidad_tabla; i++){
		if(hash->tabla[i]){ //La tabla[i] posee una lista 
			while(!lista_esta_vacia(hash->tabla[i])){
				nodo_hash_t* nodo_a_destruir = (nodo_hash_t*) lista_borrar_primero(hash->tabla[i]);
				nodo_hash_destruir(nodo_a_destruir, hash->funcion_destruir_dato);
			}
			//La lista ya quedo vaciada y los nodos destruidos
			lista_destruir(hash->tabla[i], NULL);
		}
	}
	free(hash->tabla);
	free(hash);
}

////////////////////////  A U X I L I A R E S   H A S H  //////////////////////////////

//---------------------------- Auxiliares del nodo_hash ----------------------------

nodo_hash_t *nodo_hash_crear(const char *clave, void *dato){
	nodo_hash_t* nodo_hash = malloc(sizeof(nodo_hash_t));
	if(!nodo_hash) return NULL;
	char* clave_copia = nodo_hash_guardar_clave(clave);
	if(!clave_copia){
		free(nodo_hash);
		return NULL;
	}
	nodo_hash->clave = clave_copia;
	nodo_hash->dato = dato;
	return nodo_hash;
}

char* nodo_hash_guardar_clave(const char* clave){
	size_t clave_longitud = strlen(clave);
	char* clave_copia = malloc(sizeof(char) * (clave_longitud+1));
	if(!clave_copia) return NULL;
	strcpy(clave_copia, clave);
	return clave_copia;
}

void nodo_hash_destruir(nodo_hash_t* nodo_hash, hash_destruir_dato_t funcion_destruir_dato){
	if(funcion_destruir_dato){
		funcion_destruir_dato(nodo_hash->dato);
	}
	free(nodo_hash->clave);
	free(nodo_hash);
}

//--------------- Auxiliares de la tabla para operarar sobre su lista -----------------

void tabla_setear_a_null(lista_t** tabla, size_t tamanio){
	for(size_t i=0; i < tamanio; i++){
		tabla[i] = NULL;
	}
}

bool tabla_crear_lista(hash_t *hash, size_t posicion){
	if(hash->tabla[posicion]) return false;
	lista_t* lista = lista_crear();
	if(!lista) return false;
	hash->tabla[posicion] = lista;
	return true;
}

nodo_hash_t *tabla_obtener_nodo_en_lista(lista_t *lista_en_tabla, const char* clave){
	size_t posc_en_lista_tabla = tabla_obtener_posicion_en_lista(lista_en_tabla, clave);
	if(posc_en_lista_tabla == -1) return NULL;
	lista_iter_t* iter = lista_iter_crear(lista_en_tabla);
	if(!iter) return NULL;
	// Ubico el iterador sobre el elemento buscado.
	for(int i=0; i < posc_en_lista_tabla; i++){
		if (!lista_iter_avanzar(iter)) return NULL;
	}
	nodo_hash_t* nodo_hash_buscado = (nodo_hash_t*)lista_iter_ver_actual(iter);
	lista_iter_destruir(iter);

	return nodo_hash_buscado;
}

size_t tabla_obtener_posicion_en_lista(lista_t *lista_en_tabla, const char* clave){
	lista_iter_t* iter = lista_iter_crear(lista_en_tabla);
	if(!iter) return -1; 
	size_t posicion_actual = 0;
	nodo_hash_t* nodo_hash_actual; 
	while(!lista_iter_al_final(iter)){ // Mientras la lista no se acabe
		nodo_hash_actual = (nodo_hash_t*)lista_iter_ver_actual(iter);
		if((strcmp(nodo_hash_actual->clave, clave )) == 0){ // Son iguales
			lista_iter_destruir(iter);
			return posicion_actual;
		}
		if (!lista_iter_avanzar(iter)) return -1;
		posicion_actual++;
	}
	lista_iter_destruir(iter);

	return -1; 
}

nodo_hash_t* tabla_borrar_nodo_en_lista(lista_t *lista_en_tabla, const char* clave){
	size_t posc_nodo_a_borrar = tabla_obtener_posicion_en_lista(lista_en_tabla, clave);
	if(posc_nodo_a_borrar == -1) return NULL;
	lista_iter_t* iter = lista_iter_crear(lista_en_tabla);
	if(!iter) return NULL;
	for(int i=0; i<posc_nodo_a_borrar; i++){ // Avanzo el iterador en la lista hasta la posicion que deseo borrar
		lista_iter_avanzar(iter);
	}
	nodo_hash_t* nodo_hash_borrado = (nodo_hash_t*)lista_iter_borrar(iter);

	lista_iter_destruir(iter);
	return nodo_hash_borrado;
}

//---------------------- Auxiliares para redimensionar el hash ------------------------

bool hash_redimensionar(hash_t *hash){
	size_t capacidad_nueva = hash_redimensionar_calcular_tam_nuevo(hash);
	lista_t** tabla_nueva = malloc(sizeof(lista_t*) * capacidad_nueva);
	tabla_setear_a_null(tabla_nueva, capacidad_nueva);
	size_t posc;
	if(!tabla_nueva) return false;
	for(size_t i=0; i < hash->capacidad_tabla; i++){
		if(hash->tabla[i]){
			while(!lista_esta_vacia(hash->tabla[i])){
				nodo_hash_t* nodo_hash = (nodo_hash_t*)lista_borrar_primero(hash->tabla[i]);
				posc = funcion_hash(capacidad_nueva, nodo_hash->clave);
				if(!tabla_nueva[posc]){
					tabla_nueva[posc] = lista_crear();
					if(!tabla_nueva[posc]) return false;
				}
				if(!lista_insertar_ultimo(tabla_nueva[posc], nodo_hash)) return false;
			}
			lista_destruir(hash->tabla[i], NULL);
		}
	}
	tabla_setear_a_null(hash->tabla, hash->capacidad_tabla);
	lista_t** tabla_aux = hash->tabla;
	hash->tabla = tabla_nueva;
	hash->capacidad_tabla = capacidad_nueva;
	free(tabla_aux);
	return true;
}


bool hash_analizar_necesidad_de_redimension(hash_t *hash){
	if(hash->cantidad >= (size_t)((float)hash->capacidad_tabla * (float)CARGA_MAXIMA_LISTA)){
		return true;
	}
	if(hash->cantidad <= (size_t)((float)hash->capacidad_tabla * (float)CARGA_MINIMA_LISTA)){
		return true;
	}
	return false;
}

size_t hash_redimensionar_calcular_tam_nuevo(hash_t* hash){
	size_t tam_nuevo;
	if(hash -> cantidad >= hash -> capacidad_tabla * CARGA_MAXIMA_LISTA){
		for (int i = 0; i < CANTIDAD_ELEMENTOS_ARREGLO_PRIMOS; i++){
			if (NUMEROS_PRIMOS[i] > hash -> capacidad_tabla){
				tam_nuevo = NUMEROS_PRIMOS[i];
				return tam_nuevo;
			}
		}
		tam_nuevo = hash -> capacidad_tabla;
		while(hash -> cantidad >= (size_t)((float)tam_nuevo * (float)CARGA_MAXIMA_LISTA)){
			tam_nuevo = tam_nuevo * FACTOR_REDIMENSIONAR_AGRANDAR;
		}
		return tam_nuevo;
	}
	else{
		if (hash -> capacidad_tabla == CAPACIDAD_INICIAL){
			return CAPACIDAD_INICIAL;
		}
		size_t pos_numero_primo_anterior = 0;
		for (int i = 1; i < CANTIDAD_ELEMENTOS_ARREGLO_PRIMOS; i++){
			if (NUMEROS_PRIMOS[i] == hash -> capacidad_tabla){
				tam_nuevo = NUMEROS_PRIMOS[pos_numero_primo_anterior];
				pos_numero_primo_anterior--;
				while(hash -> cantidad <= (size_t)((float)tam_nuevo * CARGA_MINIMA_LISTA)){
					if (tam_nuevo == CAPACIDAD_INICIAL){
						return CAPACIDAD_INICIAL;
					}
					tam_nuevo = NUMEROS_PRIMOS[pos_numero_primo_anterior];
					pos_numero_primo_anterior--;
				}
				return tam_nuevo;
			}
			pos_numero_primo_anterior++;
		}
		tam_nuevo = hash -> capacidad_tabla;
		while(hash -> cantidad <= (size_t)((float)tam_nuevo * CARGA_MINIMA_LISTA)){
			if (tam_nuevo <= CAPACIDAD_INICIAL){
				return CAPACIDAD_INICIAL;
			}
			tam_nuevo = tam_nuevo / FACTOR_REDIMENSIONAR_ACHICAR;
		}
		return tam_nuevo;
	}
}

//---------------------------- Auxiliares funcion hash  ----------------------------


size_t funcion_hash(size_t capacidad_hash, const char* clave){
	size_t codominio;
	size_t longitud = strlen(clave);
	codominio = SDBMHash(clave, longitud);

	return(codominio % capacidad_hash);
}

size_t SDBMHash(const char* str, size_t length) {
	unsigned int hash = 0;
	size_t i = 0;

	for (i = 0; i < length; str++, i++){
		hash = (*str) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////   I T E R A D O R   ///////////////////////////////////

struct hash_iter {
	const hash_t* hash;
	size_t posc_actual;
	lista_iter_t* lista_iter_posc_actual;
	bool esta_al_final;
};

///////////////////////  A U X I L I A R E S   I T E R A D O R  /////////////////////////
///////////////////////         D O C U M E N T A C I O N       /////////////////////////

// Busca la proxima posicion de la lista que contiene elementos.
// Pre: -
// Post: Retorna la siguiente posicion a la posc_actual que contiene elementos, de no 
// encontrar ninguna y llegar al final retorna la posc_actual sin alterar.
size_t tabla_obtener_posc_siguiente(const hash_t* hash, size_t posc_actual);

// Retorna un iterador para recorrer la lista de una posicion de la tabla
// Pre: La posicion de la tabla posee una lista.
// Post: Retorna un iterador para la lista.
lista_iter_t *tabla_obtener_lista_iter(lista_t* tabla_actual);

/////////////////////  P R I M I T I V A S   I T E R A D O R  ///////////////////////////

hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* hash_iter = malloc(sizeof(hash_iter_t));
	if(!hash_iter) return NULL;
	hash_iter->hash = hash;
	hash_iter->esta_al_final = false;
	hash_iter->posc_actual = 0;

	// Le busco una posc_actual valida (que contenga el primer valor).
	if(!hash_iter->hash->tabla[hash_iter->posc_actual]){ 
		// Si en posc 0 no hay tabla.
		size_t posc_siguiente = tabla_obtener_posc_siguiente(hash, hash_iter->posc_actual);
		if(posc_siguiente == hash_iter->posc_actual){
			// Si la posc siguiente es igual a la posc actual es por que no hay siguiente.
			// y estamos al final de la tabla. (El hash esta vacio).
			hash_iter->esta_al_final = true;
		}else{
			hash_iter->posc_actual = posc_siguiente;
		}
	}

	// Si no esta al final, le genero un lista_iter para la lista en posc_actual
	if(!hash_iter->esta_al_final){
		lista_iter_t* lista_iter = tabla_obtener_lista_iter(hash->tabla[hash_iter->posc_actual]);
		if(!lista_iter){
			hash_iter->lista_iter_posc_actual = NULL;
		}else{
			hash_iter->lista_iter_posc_actual = lista_iter;
		}	
	}else{
		hash_iter->lista_iter_posc_actual = NULL;
	}
	return hash_iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
	if(hash_iter_al_final(iter)) return false;
	lista_iter_avanzar(iter->lista_iter_posc_actual);
	if(!lista_iter_al_final(iter->lista_iter_posc_actual)){
		// Quedan elementos por recorrer en la lista de posc_actual
		return true;
	}else{
		// No quedan elementos por recorrer en la lista de posc_actual
		// avanzo a la proxima poscision
		size_t posc_siguiente = tabla_obtener_posc_siguiente(iter->hash, iter->posc_actual);
		if(posc_siguiente == iter->posc_actual){
			// Si la posc siguiente es igual a la posc actual es por que no hay siguiente.
			// y estamos al final de la tabla.
			lista_iter_destruir(iter->lista_iter_posc_actual);
			iter->lista_iter_posc_actual = NULL;
			iter->esta_al_final = true;
			return false;
		}else{
			iter->posc_actual = posc_siguiente;
			lista_iter_t* lista_iter = tabla_obtener_lista_iter(iter->hash->tabla[iter->posc_actual]);
			if(!lista_iter) return false;
			lista_iter_destruir(iter->lista_iter_posc_actual);
			iter->lista_iter_posc_actual = lista_iter;
			return true;
		}
	}
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if(iter->lista_iter_posc_actual){
		nodo_hash_t* nodo_hash= (nodo_hash_t*)lista_iter_ver_actual(iter->lista_iter_posc_actual);
		return nodo_hash->clave;
	}
	return NULL;
}

bool hash_iter_al_final(const hash_iter_t *iter){
	return iter->esta_al_final;
}

void hash_iter_destruir(hash_iter_t* iter){
	if(iter->lista_iter_posc_actual){
		lista_iter_destruir(iter->lista_iter_posc_actual);
	}
	free(iter);
}

/////////////////////  A U X I L I A R E S   I T E R A D O R  ///////////////////////////


size_t tabla_obtener_posc_siguiente(const hash_t* hash, size_t posc_actual){
	size_t posc_siguiente = posc_actual+1;

	while(posc_siguiente < hash->capacidad_tabla && !hash->tabla[posc_siguiente]){
		posc_siguiente++;
	}
	if(posc_siguiente == hash->capacidad_tabla){
		return posc_actual;
	}
	return posc_siguiente;
}

lista_iter_t *tabla_obtener_lista_iter(lista_t* tabla_actual){
	lista_iter_t* lista_iter = lista_iter_crear(tabla_actual);
	if(!lista_iter) return NULL;
	return lista_iter;
}