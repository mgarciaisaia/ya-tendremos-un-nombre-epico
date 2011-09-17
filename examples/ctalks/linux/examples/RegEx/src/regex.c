
/*	Copyright (C) 2008, 2009 Free Software Foundation, Inc.
	This file is part of C-Talks Example's.

	 C-Talks Example's is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 3, or (at your option) any later
	version.

	 C-Talks Example's is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
	for more details.

	You should have received a copy of the GNU General Public License along with
	this program. If not, see <http://www.gnu.org/licenses/>.*/

/*
 ============================================================================
 Name        : regex.c
 Author      : Jarlakxen
 Version     : 1.0
 Description : Ejemplo para usar regular expression
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

/*
 * Ejemplo tomado de:
 * http://www.opengroup.org/onlinepubs/9699919799/functions/regcomp.html
 */
int validate(char *string, char *pattern){
	int    status, regex_error;
	regex_t    re;


	if ( (regex_error=regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB)) != 0){
		char regex_strerror[200];
		regerror(regex_error, &re, regex_strerror, 200);
		puts(regex_strerror);
		return 0;      /* Report error. */
	}


	status = regexec(&re, string, (size_t) 0, NULL, 0);
	regfree(&re);

	if (status != 0)
		return 0;      /* Report error. */

	return 1;
}

int main(int argc, char **argv) {

	/* Verifico los parametros recividos */
	if( argc != 3){
		puts("Numero Invalido de argumentos, formato del comando:");
		puts("	regex <cadena> <expresion regular>\n");
		puts("Ej:");
		puts("regex \"Hola\" \"^[a-zA-Z]$\" ");
		return EXIT_SUCCESS;
	}

	/* Verifico si el string es valido */
	if( validate(argv[1], argv[2]) ){
		puts("String Valido!");
	} else {
		puts("String Invalido!");
	}

	return EXIT_SUCCESS;
}
