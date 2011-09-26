#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "almacenamiento.h"

struct configuracion {
	char *rutaAlDisco;
	int archivoDisco;
};

int main(int argc, char *argv[]) {
	struct configuracion configuracion;
	configurar(argc, argv, configuracion);
	abrirArchivoDisco(configuracion);
	return EXIT_SUCCESS;
}

/**
 * Levanta las configuraciones, abre el archivo de disco, etc
 *
 * Deja todo preparado para poder operar
 */
void configurar(int argc, char *argv[], struct configuracion configuracion) {
	if(argc != 2) {
		exitWithUsage(argc, argv);
	}

	// Ponele que con esto copiamos el primer parametro de la aplicacion
	// (el path al archivo del disco) en el struct de configuracion
	strcpy(configuracion.rutaAlDisco, argv[1]);

	// TODO: aca deberia leer el planificador.ini y seguir leyendo configuracion
}

void salirConModoDeUso(int argc, char *argv[]) {
	modoDeUso();
	perror("mala cantidad de parametros - " + argc);
	exit(EXIT_FAILURE);
}

void modoDeUso(void) {
	printf("*** Modo de uso ***\n");
	printf("\tplanificador PATH_AL_DISCO\n");
	printf("\nPor ejemplo:\n");
	printf("$ ./planificador fat32.disk\n");
}

void abrirArchivoDisco(struct configuracion configuracion) {
	configuracion.archivoDisco = open(configuracion.rutaAlDisco, NULL);
}

void leerSector(int32_t numeroSector, fat32_sector data, struct configuracion configuracion) {
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
	data = mmap((caddr_t) 0, sizeof fat32_sector, PROT_READ, MAP_SHARED, configuracion.archivoDisco, getpagesize());

	// FIXME: implementar
}

void escribirSector(int32_t numeroSector, fat32_sector data, struct configuracion configuracion) {
	// FIXME: implementar
}
