/*
 * Copyright 2010 Matias Dumrauf
 *
 * This file is part of C Talks Example's.
 *
 * C Talks Example's is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * C Talks Example's is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with C Talks Example's. If not, see http://www.gnu.org/licenses/.
 *
 */

/*
 ============================================================================
 Name        : persona.c
 Author      : Matias Dumrauf
 Version     : 1.0
 Created on  : 03/09/2010
 Copyright   : C Talks
 Description : Parte privada del TAD Persona.
			   Contiene la implementacion del TAD.
 ============================================================================
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
	return !strcmp(p1->documento.tipo, p2->documento.tipo) && p1->documento.numero == p2->documento.numero &&
		   !strcmp(p1->apellido, p2->apellido) && !strcmp(p1->nombre, p2->nombre) && p1->sexo == p2->sexo;
}

void	   persona_saludarA(t_persona *p){
	printf("Hola %s, %s! Como estas?\n", p->nombre, p->apellido);
}

void 	   persona_cumplirAnos(t_persona *p){
	p->edad++;
}

int		   persona_esJubilado(t_persona *p){
	return p->sexo == 'M' ? p->edad >= 65 : p->edad >= 60;
}

void	   persona_maldecir(void){
	printf("Me cache en Die!!!");
}

t_persona *persona_falsificarIdentidad(t_persona *p, char *nuevoApellido, char *nuevoNombre){
	free(p->apellido);
	free(p->nombre);

	p->apellido = malloc(strlen(nuevoApellido) + 1);
	strcpy(p->apellido, nuevoApellido);
	p->nombre = malloc(strlen(nuevoNombre) + 1);
	strcpy(p->nombre, nuevoNombre);

	return p;
}
