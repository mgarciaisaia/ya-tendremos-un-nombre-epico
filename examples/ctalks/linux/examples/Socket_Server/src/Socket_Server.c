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
 Name        : Socket_Client.c
 Author      : Jarlakxen
 Version     : 1.0
 Description : Servidor red usando Sockets
 ============================================================================
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SOCKET_MAX_BUFFER 100

int main(int argc, char **argv) {
	int addrlen = sizeof(struct sockaddr_in);
	char buffer[SOCKET_MAX_BUFFER];

	/* Creo el Socket: SOCK_STREAM para TCP y SOCK_DGRAM par UDP */
	int descriptor = socket(AF_INET, SOCK_STREAM, 0);
	int remote_client;

	/* Direccion Local */
	struct sockaddr_in *local_address = malloc(sizeof(struct sockaddr_in));
	/* Direccion remota ( a la que me quiero conectar )*/
	struct sockaddr_in *remote_address = malloc(sizeof(struct sockaddr_in));

	{	/* Con esto fuerzo a que el puerto local sea el 5201 y que tome la IP por defecto de la PC */
		local_address->sin_family = AF_INET;
		local_address->sin_addr.s_addr = inet_addr("127.0.0.1");
		local_address->sin_port = htons(5300);

		bind(descriptor, (struct sockaddr *)local_address, sizeof(struct sockaddr_in));
	}

	/* Activo la escucha de conexiones entrantes a traves del puerto 5200 y como maximo 100 conexiones.*/
	listen(descriptor, 100);

	/* Acepto nueva conexion entrante */
	remote_client = accept(descriptor, (struct sockaddr *)remote_address, (void *)&addrlen);

	/* Leo e imprimo la informacion recivida a traves de la nueva conexion */
	recv(remote_client, buffer, SOCKET_MAX_BUFFER, 0);
	printf("%s", buffer);

	/* Cierro el socket y por ende la conexion */
	close(descriptor);

	free(local_address);
	free(remote_address);

	return EXIT_SUCCESS;
}
