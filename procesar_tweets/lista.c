#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
	void* dato;
	struct nodo* siguiente;
}nodo_t;

struct lista {
	nodo_t* primero;
	nodo_t* ultimo;
	size_t largo;
};

// Crea una cola.
// Post: devuelve un nuevo nodo vacío con su respectivo dato en su interior.
// Si no ha sido posible retorna NULL.
nodo_t* nodo_crear(void* dato);

lista_t *lista_crear(void){
	lista_t* lista = malloc(sizeof(lista_t));
	if (lista == NULL) return NULL;
	lista->primero = NULL;
	lista->primero = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista){
	return (lista_largo(lista) == 0);
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t* nodo_nuevo = nodo_crear(dato);
	if (nodo_nuevo == NULL) return false;

	if(lista_esta_vacia(lista)){	
		lista->ultimo = nodo_nuevo;		
	}
	else{
		nodo_nuevo->siguiente = lista->primero;
	}
	lista->primero = nodo_nuevo;	
	lista->largo++;

	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t* nodo_nuevo = nodo_crear(dato);
	if (nodo_nuevo == NULL) return false;

	if(lista_esta_vacia(lista)){
		lista->primero = nodo_nuevo;	
	}
	else{
		lista->ultimo->siguiente = nodo_nuevo;
	}
	lista->ultimo = nodo_nuevo;
	lista->largo++;	

	return true;
}

void *lista_borrar_primero(lista_t *lista){
	if(lista_esta_vacia(lista)) return NULL;

	nodo_t* primero_a_borrar = lista->primero;
	void* dato_primero_a_borrar = primero_a_borrar->dato;

	if(lista->primero == lista->ultimo){
		lista->ultimo = NULL;
	}
	lista->primero = lista->primero->siguiente;
	lista->largo--;

	free(primero_a_borrar);
	return dato_primero_a_borrar;
}

void *lista_ver_primero(const lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	return lista->primero->dato;
}
void *lista_ver_ultimo(const lista_t* lista){
	if (lista_esta_vacia(lista)) return NULL;
	return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}


void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	while(!lista_esta_vacia(lista)){
		if(destruir_dato){
			destruir_dato(lista->primero->dato);
		}
		lista_borrar_primero(lista);
	}
	free(lista);
}

nodo_t* nodo_crear(void* dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(nodo == NULL) return NULL;
	nodo->dato = dato;
	nodo->siguiente = NULL;
	return nodo;
}


/*   I T E R A D O R   E X T E R N O   */

struct lista_iter{
	lista_t* lista;
	nodo_t* anterior;
	nodo_t* actual;
};


lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t* iterador = malloc(sizeof(lista_iter_t));
	if (iterador == NULL) return NULL;
	iterador->lista = lista;
	iterador->anterior = NULL;
	iterador->actual = lista->primero;
	return iterador;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if(lista_iter_al_final(iter)) return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->siguiente;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if(lista_iter_al_final(iter)) return NULL;
	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return (iter->actual == NULL);
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){	
	if (iter->anterior == NULL){ // Inserto al principio
		if (lista_insertar_primero(iter->lista, dato)){
			iter->actual = iter->lista->primero;
			return true;
		}
		return false;
	}
	if (lista_iter_al_final(iter)){ // Inserto al final
		if (lista_insertar_ultimo(iter->lista, dato)){
			iter->actual = iter->lista->ultimo;
			return true;
		} 
		return false;
	}
	// Inserto en el medio
	nodo_t* nodo_nuevo = nodo_crear(dato);
	if (nodo_nuevo == NULL) return false;

	iter->anterior->siguiente = nodo_nuevo;
	nodo_nuevo->siguiente = iter->actual;
	iter->actual = nodo_nuevo;
	iter->lista->largo++;

	return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
	void* dato_aux = NULL;
	if (lista_iter_al_final(iter)) return dato_aux;
	//Borrar primero
	if (iter->anterior == NULL){ 
		iter->actual = iter->actual->siguiente;
		dato_aux = lista_borrar_primero(iter->lista);
	}
	//Borrar en medio o ultimo
	else {
		dato_aux = lista_iter_ver_actual(iter); 
		iter->actual = iter->actual->siguiente; 
		free(iter->anterior->siguiente); 
		iter->anterior->siguiente = iter->actual; 
		if (lista_iter_al_final(iter)){
			iter->lista->ultimo = iter->anterior;
		}	
		iter->lista->largo--;
	}
	return dato_aux;
} 

/*   I T E R A D O R   I N T E R N O   */

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	bool iterar = true;
	nodo_t* actual = lista->primero;
	while(iterar && actual!=NULL){
		iterar = visitar(actual->dato, extra);
		actual = actual->siguiente;
	}
}