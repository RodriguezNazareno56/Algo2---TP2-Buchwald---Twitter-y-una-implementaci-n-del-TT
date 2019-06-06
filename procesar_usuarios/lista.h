#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

struct lista;
typedef struct lista lista_t;


 /* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea un lista.
// Post: devuelve una nueva lista vacia.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento a al principio de la lista. Devuelve falso en caso
// de error.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento a la lista, el cual se encuentra al principio
// de la lista
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento a al final de la lista. Devuelve falso en caso
// de error.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento a la lista, el cual se encuentra al final
// de la lista
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Borra el primer elemento de la lista, retornando su valor.
// Pre: la lista fue creada.
// Post: se elimino el primer elemento de la lista. Se obtiene como retorno
// el valor del dato contenido.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primero elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si esta vacia devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el primer elemento de la lista, cuando no esta vacia.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si esta vacia devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el ultimo elemento de la lista, cuando no esta vacia.
void *lista_ver_ultimo(const lista_t* lista);

// Obtiene el largo de la lista.
// Pre: la lista fue creada.
// Post: se devuelve la cantidad de elementos que posee la lista.
size_t lista_largo(const lista_t *lista);

// Destruyo la lista. Si se recibe la funcion destruir_dato por parametro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una funcion capaz de destruir 
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/* ******************************************************************
 *               PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/

// Itera sobre la lista pasada por parametro haciendo uso de la funcion "visitar"(pasada
// por parametro) operando sobre cada elemento de la lista hasta que la funcion vesitar
// retorne false. 
// Pre: la lista fue creada
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/* ******************************************************************
 *                PRIMITIVAS DE ITERADOR EXTERNO
 * *****************************************************************/

struct lista_iter;
typedef struct lista_iter lista_iter_t;

// Crea un iterador externo.
// Pre: la lista fue creada.
// Post: el iterador apunta a la primer posicion de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza el iterador un posicion en la lista.
// Pre: el iterador y la lista fueron creados.
// Post: el iterador avanzo una posicion. Retorna false si no se puede
// continuar avanzando.
bool lista_iter_avanzar(lista_iter_t *iter);

// Retorna el dato al que apunta el iterador en al lista.
// Pre: el iterador y la lista fueron creados.
// Post: devuelve el valor de la lista al que apunta el iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Comprueba que el iterador se encuentre al final de la lista.
// Pre: el iterador y la lista fueron creados.
// Post: devuelve true si el iterador se encuentra al final.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye al iterador.
// Pre: el iterador fue creado.
// Post: se destruyo al iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento en la lista en la posicion en la que el iterador se encuentra.
// Devuelve true si logra insertarlo, false en caso de no poder.
// Pre: el iterador y la lista fueron creados
// Post: se inserto el elemento en la lista
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Elimina el elemento de la lista que se encuentra en la posicion en la que el iterador
// apunta y lo retorna, 
// Pre: el iterador y la lista fueron creados
// post: se elimino el elemento de la lista
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

#endif // LISTA_H