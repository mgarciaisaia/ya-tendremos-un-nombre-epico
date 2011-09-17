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
 Name        : main_persona.c
 Author      : Matias Dumrauf
 Version     : 1.0
 Created on  : 14/09/2010
 Copyright   : C Talks
 Description : Programa ejemplo del uso del TAD Persona.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "persona.h"

int main(void) {
	t_persona *p1 = persona_crear("Dumrauf", "Matias", "DNI", 33123456, 'M', 22);
	t_persona *p2 = persona_crear("Dumrauf", "Matias", "DNI", 33123456, 'M', 22);

	printf("- Persona 1 -\nApellido: %s\nNombre: %s\nEdad: %d\nTipo y N° doc: %s %d\nSexo: %s\n\n",
			p1->apellido, p1->nombre, p1->edad, p1->documento.tipo, p1->documento.numero,
			p1->sexo == 'M' ? "Masculino" : "Femenino");

	printf("- Persona 2 -\nApellido: %s\nNombre: %s\nEdad: %d\nTipo y N° doc: %s %d\nSexo: %s\n\n",
			p2->apellido, p2->nombre, p2->edad, p2->documento.tipo, p2->documento.numero,
			p2->sexo == 'M' ? "Masculino" : "Femenino");

	if (persona_equals(p1, p2))
		printf("Las dos personas son iguales.\n\n");
	else
		printf("Las dos personas son distintas.\n\n");

	persona_cumplirAnos(p1);
	persona_falsificarIdentidad(p2, "Smithers", "Castulo");
	p2->sexo = 'F';

	printf("- Persona 1 -\nApellido: %s\nNombre: %s\nEdad: %d\nTipo y N° doc: %s %d\nSexo: %s\n\n",
			p1->apellido, p1->nombre, p1->edad, p1->documento.tipo, p1->documento.numero,
			p1->sexo == 'M' ? "Masculino" : "Femenino");

	printf("- Persona 2 -\nApellido: %s\nNombre: %s\nEdad: %d\nTipo y N° doc: %s %d\nSexo: %s\n\n",
			p2->apellido, p2->nombre, p2->edad, p2->documento.tipo, p2->documento.numero,
			p2->sexo == 'M' ? "Masculino" : "Femenino");

	if (persona_equals(p1, p2))
		printf("Las dos personas son iguales.\n");
	else
		printf("Las dos personas son distintas.\n");

	persona_saludarA(p2);
	persona_maldecir();

	persona_matar(p1);
	persona_matar(p2);

	return EXIT_SUCCESS;
}
