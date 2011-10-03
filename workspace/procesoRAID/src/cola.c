#include <stdio.h>
#include <stdlib.h>

typedef struct nodo nodo;
typedef struct cola cola;

struct cola {
	nodo * inicio;
	nodo * fin;
	agregar * = agregar;
};

struct nodo {
	void * contenido;
	nodo * siguiente;
};

void agregar(void * contenido, cola * cola) {
	nodo * nodo = malloc(sizeof nodo);
	nodo->contenido = contenido;
	nodo->siguiente = NULL;
	if (cola->inicio == NULL) {
		cola->inicio = nodo;
		cola->fin = nodo;
	} else {
		cola->fin->siguiente = nodo;
		cola->fin = nodo;
	}
}

void * sacar(void * contenido, cola * cola) {
	nodo * anterior;
	nodo * actual = cola->inicio;
	if (actual != NULL) {
		if (actual->contenido == contenido) {
			if (cola->inicio == cola->fin) {
				cola->inicio = NULL;
				cola->fin = NULL;
			} else {
				cola->inicio = actual->siguiente;
			}
			free(actual);
			return contenido;
		} else {
			while (actual != NULL) {
				anterior = actual;
				actual = actual->siguiente;
				if (actual->contenido == contenido) {
					anterior->siguiente = actual->siguiente;
					if (actual->siguiente == NULL) {
						cola->fin = anterior;
					}
					free(actual);
					return contenido;
				}
			}
		}

	}
	return NULL;
}

void limpiar(cola * cola) {
	while (cola->inicio != NULL) {
		sacar(cola->inicio->contenido, cola);
	}

}
