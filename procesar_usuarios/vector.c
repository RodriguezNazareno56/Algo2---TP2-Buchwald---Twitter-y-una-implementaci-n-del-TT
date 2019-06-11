	#include "vector.h"


// Funciones del alumno.



// Valida la existencia de la posicion solicitada en el vector de datos
// Pre: el vector fue creado
// Post: retorna true si la posicion se encuentra en el vector de datos, 
// retorna false si la posicion se encuentra fuera de rango.
bool validar_posicion(vector_t* vector, size_t pos);

void vector_destruir(vector_t* vector){
	free(vector->datos);
	free(vector);
}

bool vector_obtener(vector_t* vector, size_t pos, int* valor){
	if(!validar_posicion(vector, pos)){ 
		return false; //pos fuera de rango
	}
	*valor = vector->datos[pos];
	return true;
}


bool vector_guardar(vector_t* vector, size_t pos, int valor){
	if(!validar_posicion(vector, pos)){
		return false;//pos fuera de rango
	}
	vector->datos[pos] = valor;
	vector->cant++;
	return true; 
}

size_t vector_largo(vector_t* vector){
	return vector->tam;
}

size_t vector_cantidad(vector_t* vector){
	return vector->cant;
}

bool validar_posicion(vector_t* vector, size_t pos){
	if(pos < vector->tam){ //  0 a tam-1
		return true;
	}
	return false; //pos fuera de rango
}

// Funciones implementadas por la catedra.

vector_t* vector_crear(size_t tam) {
	vector_t* vector = malloc(sizeof(vector_t));

	if (vector == NULL) {
		return NULL;
	}
	vector->datos = calloc(tam, sizeof(int));

	if (tam > 0 && vector->datos == NULL) {
		free(vector);
		return NULL;
	}
	vector->tam = tam;
	vector->cant = 0;
	return vector;
}

bool vector_redimensionar(vector_t* vector, size_t tam_nuevo) {
	size_t tam_anterio = vector->tam;
	int* datos_nuevo = realloc(vector->datos, tam_nuevo * sizeof(int));
	for(size_t i = tam_anterio; i < tam_nuevo; i++){
		datos_nuevo[i] = 0;
	}
	// Cuando tam_nuevo es 0, es correcto si se devuelve NULL.
	// En toda otra situación significa que falló el realloc.
	if (tam_nuevo > 0 && datos_nuevo == NULL) {
		return false;
	}

	vector->datos = datos_nuevo;
	vector->tam = tam_nuevo;
	return true;
}
