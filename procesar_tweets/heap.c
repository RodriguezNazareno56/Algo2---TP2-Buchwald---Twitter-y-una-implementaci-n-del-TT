#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define CAPACIDAD_INICIAL 30
#define FACTOR_REDIMENSION 2
#define FACTOR_ACHICAMIENTO 3

////////////////////////  E S T R U C T U R A   H E A P  //////////////////////////////

struct heap{
    void** datos;
    size_t cant;
    size_t capacidad;
    cmp_func_t cmp;
};

////////////////////////  A U X I L I A R E S   H E A P  //////////////////////////////
////////////////////////    D O C U M E N T A C I O N    //////////////////////////////

void swap(void** vector, size_t posicion_1, size_t posicion_2){
    void* aux = vector[posicion_1];
    vector[posicion_1] = vector[posicion_2];
    vector[posicion_2] = aux;
}

void upheap(void** datos, size_t posicion, cmp_func_t cmp){
    if (posicion == 0) return;
    size_t posicion_padre = (posicion - 1) / 2;
    if (cmp(datos[posicion_padre], datos[posicion]) < 0){
        swap(datos, posicion, posicion_padre);
        upheap(datos, posicion_padre, cmp);
    }
}

void downheap(heap_t* heap, size_t posicion){
    if (posicion >= heap -> cant) return;
    size_t min = posicion; // PADRE
    size_t izq = 2 * posicion + 1;
    size_t der = 2 * posicion + 2;
    if (izq < heap -> cant && heap -> cmp(heap -> datos[izq], heap -> datos[min]) > 0){
        min = izq;
    }
    if (der < heap -> cant && heap -> cmp(heap -> datos[der], heap -> datos[min]) > 0){
        min = der;
    }
    if (min != posicion){
        swap(heap -> datos, posicion, min);
        downheap(heap, min);
    }
}

heap_t *heapify(void **datos, size_t cant, cmp_func_t cmp){
    heap_t* heap = heap_crear(cmp);
    free(heap -> datos);
    heap -> datos = datos;
    heap -> capacidad = cant;
    heap -> cant = cant;
    heap -> cmp = cmp;
    int actual = ((int)cant) / 2 - 1;
    while(actual >= 0){
        downheap(heap, actual);
        actual--;
    }
    return heap;
}

bool heap_redimensionar(heap_t *heap, size_t capacidad_nueva){
   void **datos_redimensionados = realloc(heap -> datos, sizeof(void*) * capacidad_nueva);
   if(!datos_redimensionados) return false;
   heap -> datos = datos_redimensionados;
   heap -> capacidad = capacidad_nueva;
   return true;
}

////////////////////////  P R I M I T I V A S   H E A P  //////////////////////////////

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heap_t *heap = heapify(elementos, cant, cmp);
    while(heap -> cant > 1){
        swap(heap -> datos, 0, heap -> cant - 1);
        heap -> cant--;
        downheap(heap, 0);
    }
    free(heap);
}

heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;
    void** datos = calloc(CAPACIDAD_INICIAL, sizeof(void*));
    if (!datos){
        free(heap);
        return NULL;
    }
    heap -> datos = datos;
    heap -> cmp = cmp;
    heap -> cant = 0;
    heap -> capacidad = CAPACIDAD_INICIAL;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	void **arreglo_copia = malloc(n * sizeof(char*));
	for(size_t i = 0; i < n; i++){
		arreglo_copia[i] = arreglo[i];
	} 
	return  heapify(arreglo_copia, n, cmp);
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
    if (destruir_elemento){
        for (size_t i = 0; i < heap -> cant; i++){
            destruir_elemento(heap -> datos[i]);
        }
    }
    free(heap -> datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap -> cant;
}

bool heap_esta_vacio(const heap_t *heap){
    return heap_cantidad(heap) == 0;
}

bool heap_encolar(heap_t *heap, void *elem){
    if (!elem) return false;
    if (heap_cantidad(heap) == heap -> capacidad){
        heap_redimensionar(heap, heap -> capacidad * FACTOR_REDIMENSION);
    }
    heap -> datos[heap -> cant] = elem;
    heap -> cant++;
    upheap(heap -> datos, heap -> cant - 1, heap -> cmp);
    return true;
}

void *heap_ver_tope(const heap_t *heap){
    if (heap_esta_vacio(heap)) return NULL;
    return heap -> datos[0];
}

void *heap_desencolar(heap_t *heap){
    if (heap_cantidad(heap) == 0) return NULL;
    void* dato_aux = heap -> datos[0];
    swap(heap -> datos, 0, heap -> cant - 1);
    heap -> cant--;
    downheap(heap, 0);
    if (heap_cantidad(heap) > CAPACIDAD_INICIAL && heap_cantidad(heap) < heap -> capacidad / FACTOR_ACHICAMIENTO){
        heap_redimensionar(heap, heap -> capacidad / FACTOR_REDIMENSION);
    }
    return dato_aux;
}

void heap_actualizar(heap_t *heap){
    downheap(heap, 0);
}