#include "pila.h"
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...
#define TAMANIO_INICIAL 50
#define FACTOR_REDIMENSIONAR_AGRANDAR 2
#define FACTOR_REDIMENSIONAR_ACHICAR 2

// Analiza si resulta necesario redimensionar la pila (aumentar o reducir
// el tamaño de su "vector->datos"). De ser así, calcula el nuevo tamaño y hace
// uso de la funcion pila_redimensionar.
// Pre: La pila fue creada.
// Post: Retorna false si el redimensionamiento era necesario y no ha sido 
// posible realizarlo. Retorna true si el redimensionamiento se concreto con
// exito o no resulta necesario llevarlo a cabo.
bool analizar_redimensionamiento(pila_t *pila);

// Redimensiona el tamaño de vector "pila->datos"
// Pre: La pila fue creada y el nuevo tamaño calculado.
// Post: Retorna true si el vector a cambiado su tamaño a tamanio_nuevo.
// Retorna false si el redimensionamiento no ha sido posible y conserva
// el arreglo original.
bool pila_redimensionar(pila_t *pila, size_t tamanio_nuevo);

pila_t* pila_crear(void){
	pila_t* pila = malloc(sizeof(pila_t));
	if(pila == NULL){
		return NULL;
	}
	pila->datos = malloc(TAMANIO_INICIAL * sizeof(void*));
	if(pila->datos == NULL){
		return NULL;
	}
	pila->capacidad = TAMANIO_INICIAL;
	pila->cantidad = 0;

	return pila;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
	return (!pila->cantidad);
}

bool pila_apilar(pila_t *pila, void* valor){
	if(!analizar_redimensionamiento(pila)){
		return false;
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad += 1;
	return true; 
}

void* pila_ver_tope(const pila_t *pila){
	if(pila_esta_vacia(pila)){
		return NULL;
	}
	return pila->datos[pila->cantidad-1]; //Ultima posicion con datos
}

void* pila_desapilar(pila_t *pila){
	if(pila_esta_vacia(pila)){
		return NULL;
	}
	analizar_redimensionamiento(pila);
	void* dato_desapilado = pila_ver_tope(pila);
	pila->cantidad -= 1;
	return dato_desapilado;
}

bool analizar_redimensionamiento(pila_t *pila){
	// Analizo si debo aumentar su tamaño.
	if(pila->cantidad == pila->capacidad){ 
		size_t tamanio_nuevo = pila->capacidad * FACTOR_REDIMENSIONAR_AGRANDAR;
		if(!pila_redimensionar(pila, tamanio_nuevo)){ 
			return false; //fallo la redimension, no se podra agregar valor a datos.
		}
	}
	// Analizo si debo reducir su tamaño.
	if(pila->cantidad - 1 <= (pila->capacidad / 4)){
		size_t tamanio_nuevo = pila->capacidad / FACTOR_REDIMENSIONAR_ACHICAR;
		if(tamanio_nuevo > TAMANIO_INICIAL){
			if(!pila_redimensionar(pila, tamanio_nuevo)){
				return false; //fallo la redimension al intentar reducir tamaño.
			}
		}	
	}

	return true; //La redimension se concreto con exito o no resulta necesaria.
}

bool pila_redimensionar(pila_t *pila, size_t tamanio_nuevo){
	void* datos_redimensionado = realloc(pila->datos,tamanio_nuevo * sizeof(void*));
	if(datos_redimensionado == NULL){
		return false;
	}
	pila->datos = datos_redimensionado;
	pila->capacidad = tamanio_nuevo;
	return true;
}
