/*
 * Copyright 2010 Jarlakxen
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
 Name        : Strings.c
 Author      : Jarlakxen
 Version     : 1.0
 Copyright   : C Talks
 Description : Este es un ejemplo de String, el cual muestra 3 formas distintas de
 	 	 	   inicializar un "String" mostrando como es que este queda dispuesto
			   en la memoria.
			   Remitirse a la presentación C Talks I, en la parte de Manejo de Memoria
			   puntualmente Array (Slide 23) y String (Slide 24).
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Al correr el ejemplo se observa que:
 *
 * - La dirección de str1 no es la misma dirección que la contenida en str1.
 *
 * - La dirección de str3 no es la misma dirección que la contenida en str3.
 *
 * - La dirección de str2 es la misma dirección que la contenida en str2.
 *
 * - El contenido de str2 es el mismo valor que el de la primera dirección de str2 (str2[0]).
 *
 */

int main(void) {

	/* Inicializamos 3 Strings de 3 formas diferentes */

	char *str1 = "Hola Mundo";

	char str2[] = "Hola Mundo";

	char *str3 = malloc( strlen("Hola Mundo") + 1 );
	strcpy(str3, "Hola Mundo");

	/* Primer Forma:
	 *
	 * Inicializamos un char*, es decir una variable que contiene una direccion a un bloque de memoria
	 * que contiene una secuencia de caracteres terminados en '\0', desde un Literal cadena.
	 */
	puts("Info str1:");

	printf("str1 => %p\n", str1);
	printf("&str1 => %p\n", &str1);
	printf("*str1 => %c\n", *str1);
	printf("%s\n", str1);

	/* Segunda Forma:
	 *
	 * Declaramos un array, es decir una secuencia contigua de elementos del mismo tipo,
	 * en el cual cada elemento del array contiene cada uno de los caracteres de literal cadena "Hola Mundo" mas el '\n'.
	 */
	puts("\nInfo str2:");

	printf("str2 => %p\n", str2);
	printf("&str2 => %p\n", &str2);
	printf("*str2 => %c\n", *str2);
	printf("%s\n", str2);


	/* Tercer Forma:
	 *
	 * Es similar a la primer forma, pero para este caso el puntero no es inicializado desde un literal cadena sino que
	 * se hace a partir de asigancion de memoria dinamica y la copia de el literal cadena "Hola Mundo" junto con el '\0'
	 * en esta area de memoria.
	 */
	puts("\nInfo str3:");

	printf("str3 => %p\n", str3);
	printf("&str3 => %p\n", &str3);
	printf("*str3 => %c\n", *str3);
	printf("%s\n", str3);

	/* Liberamos la memoria que reservamos para str3, para este caso particular ya que el programa finaliza no es necesario liberar la memoria.
	 * Ya que al finalizar el programa toda la memoria se libera, pero esto queda a manera de recordatorio
	 */
	free(str3);

	return EXIT_SUCCESS;
}
