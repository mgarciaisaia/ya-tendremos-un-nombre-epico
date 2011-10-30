#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "configuracion.h"

#ifndef TAMANIO_LINEA_CONFIGURACION
#define TAMANIO_LINEA_CONFIGURACION 200
#endif

#ifndef STRCMP_SAME_STRING
#define STRCMP_MISMA_CADENA 0
#endif

string valorConfiguracion(FILE * archivo, string clave) {
	char linea[TAMANIO_LINEA_CONFIGURACION];
	fseek(archivo, 0, SEEK_SET);
	while (!feof(archivo) && fgets(linea, TAMANIO_LINEA_CONFIGURACION, archivo) != (char *) -1) {
		/**
		 * strtok tokeniza el primer string dividiendo con el segundo string. Para conseguir los tokens siguientes,
		 * se le pasa NULL en el primer parametro. (man 3 strtok dice "Nunca use estas funciones", ja!)
		 */
		string claveLeida = strtok(linea, "=");
		if (claveLeida != NULL) {
			if (strcmp(clave, claveLeida) == STRCMP_MISMA_CADENA) {
				string valor = strtok(NULL, ";");
				string dato = malloc(sizeof valor + 1);
				strcpy(dato,valor);
				//if(str)
				// FIXME: esto tiene que ir por logging en debug/info
				printf("El valor de %s es %s", clave, dato);
				return dato;
			}
		}
	}
	return NULL;
}