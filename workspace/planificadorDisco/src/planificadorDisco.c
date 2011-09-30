#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "almacenamiento.h"

typedef char* string;

struct configuracion {
	string rutaAlDisco;
	int archivoDisco;
};


void modoDeUso(string executable) {
	printf("*** Modo de uso ***\n");
	printf("\tplanificador PATH_AL_DISCO\n");
	printf("\nPor ejemplo:\n");
	printf("$ %s fat32.disk\n", executable);
}

void salirConModoDeUso(int argc, string argv[]) {
	modoDeUso(argv[0]);
	perror("mala cantidad de parametros");
	exit(EXIT_FAILURE);
}

/**
 * Levanta las configuraciones, abre el archivo de disco, etc
 *
 * Deja todo preparado para poder operar
 */
void configurar(int argc, string argv[], struct configuracion *configuracion) {
	if(argc != 2) {
		salirConModoDeUso(argc, argv);
	}

	(*configuracion).rutaAlDisco = argv[1];
	// TODO: aca deberia leer el planificador.ini y seguir leyendo configuracion
}

void abrirArchivoDisco(struct configuracion *configuracion) {
	(*configuracion).archivoDisco = open((*configuracion).rutaAlDisco, O_RDWR);
	perror("abrirARchivo");
}

void leerSector(int32_t numeroSector, fat32_sector *data, struct configuracion *configuracion) {
	/*
	 * FIXME: esta linea que sigue ESTA MAL. No tiene que leer "sizeof fat32_sector"
	 *
	 * mmap mapea con un offset de a paginas (getpagesize()), y nosotros tenemos que leer sectores,
	 * que son mas chicos (nuestros sectores son de 512b, mientras que el pagesize dicen que suele ser de 4k).
	 *
	 * Entonces, hay que hacer alguna cochinada para leer la página en la que esta nuestro sector, y despues
	 * buscar el sector ahi adentro.
	 *
	 * Osea, ACA va la cache :) (salvo que en el campus virtual esten hablando de lo equivocados que estamos,
	 * en cuyo caso no tengo idea que tendriamos que hacer)
	 */
	char *dataFile = mmap((caddr_t) 0, getpagesize(), PROT_READ, MAP_SHARED, (*configuracion).archivoDisco, 0);
	if(dataFile == (caddr_t) -1) {
		perror("mmap de read sector");
		exit(1);
	}
	memcpy(data, dataFile + (numeroSector * 512), 512);
	munmap(dataFile, 512);
}

void escribirSector(int32_t numeroSector, fat32_sector data, struct configuracion configuracion) {
	char *dataFile = mmap((caddr_t) 0, getpagesize(), PROT_WRITE, MAP_SHARED, configuracion.archivoDisco, 0);
	if(dataFile == (caddr_t) -1) {
		perror("mmap de write sector");
		exit(1);
	}
	memcpy(dataFile + (numeroSector * 512), data, 512);
	munmap(dataFile, 512);
}

int main(int argc, string *argv) {
	struct configuracion configuracion;
	printf("argv[0]: %p\n%s\n", argv, *argv);
	configurar(argc, argv, &configuracion);
	//printf("Bleh: %s\n%s", argv[1], configuracion.rutaAlDisco);
	//exit(0);
	abrirArchivoDisco(&configuracion);
    fat32_sector data;
	leerSector(0, &data, &configuracion);
	printf("Data: %s", data);
	return EXIT_SUCCESS;
}