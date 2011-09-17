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
 Name        : persona.h
 Author      : Matias Dumrauf
 Version     : 1.0
 Created on  : 03/09/2010
 Copyright   : C Talks
 Description : Parte publica del TAD Persona.
			   Contiene las declaraciones del TAD.
 ============================================================================
 */

#ifndef PERSONA_H_
#define PERSONA_H_

	typedef struct{
		char  tipo[5];
		int   numero;
	} t_documento;

	typedef struct{
		char        *apellido;
		char        *nombre;
		t_documento  documento;
		char         sexo;
		int          edad;
	} t_persona;

	t_persona *persona_crear(char *apellido, char *nombre, char *tipo_doc, int num_doc, char sexo, int edad);
	void	   persona_matar(t_persona *p);
	int		   persona_equals(t_persona *p1, t_persona *p2);

	void	   persona_saludarA(t_persona *p);
	void 	   persona_cumplirAnos(t_persona *p);

	int	       persona_esJubilado(t_persona *p);
	void	   persona_maldecir(void);
	t_persona *persona_falsificarIdentidad(t_persona *p, char *nuevoApellido, char *nuevoNombre);

#endif /* PERSONA_H_ */
