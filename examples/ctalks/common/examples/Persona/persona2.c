/*
 * persona2.c
 *
 *  Created on: 03/09/2010
 *      Author: Matias Dumrauf
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persona.h"

t_persona *persona_crear(char *apellido, char *nombre, char *tipo_doc, int num_doc, char sexo, int edad){
	t_persona *p = malloc(sizeof(t_persona));

	p->apellido = malloc(strlen(apellido) + 1);
	strcpy(p->apellido, apellido);
	p->nombre = malloc(strlen(nombre) + 1);
	strcpy(p->nombre, nombre);
	strcpy(p->documento.tipo, tipo_doc);
	p->documento.numero = num_doc;
	p->sexo = sexo;
	p->edad = edad;

	return p;
}

void	   persona_matar(t_persona *p){
	free(p->apellido);
	free(p->nombre);
	free(p);
}

int		   persona_equals(t_persona *p1, t_persona *p2){
	return !strcmp(p1->documento.tipo, p2->documento.tipo) && p1->documento.numero == p2->documento.numero;
}

void	   persona_saludarA(t_persona *p){
	printf("Que hace' %s, %s! Como andas tanto tiempo??\n"
		   "No nos vemos desde la ultima cursada de Operativos xD.", p->nombre, p->apellido);
}

void 	   persona_cumplirAnos(t_persona *p){
	p->edad++;
}

int		   persona_esJubilado(t_persona *p){
	/* Como aca si nunca aporto, se jubila igual.. */
	return p->sexo == 'M' ? p->edad >= 45 : p->edad >= 40;
}

void	   persona_maldecir(void){
	printf("Jesus, Maria y la repu#$&? que lo p&#?!");
}

t_persona *persona_falsificarIdentidad(t_persona *p, char *nuevoApellido, char *nuevoNombre){
	free(p->apellido);
	free(p->nombre);

	p->apellido = malloc(strlen(nuevoApellido) + 1);
	strcpy(p->apellido, nuevoApellido);
	p->apellido = malloc(strlen(nuevoNombre) + 1);
	strcpy(p->nombre, nuevoNombre);

	return p;
}
