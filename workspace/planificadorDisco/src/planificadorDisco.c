#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "almacenamiento.h"

static const int SECTOR_SIZE = 512;
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
	configuracion->archivoDisco = open(configuracion->rutaAlDisco, O_RDWR);
	perror("abrirARchivo");
}

void leerSector(int32_t numeroSector, fat32_sector *data, struct configuracion *configuracion) {
	char *dataFile = mmap((caddr_t) 0, getpagesize(), PROT_READ, MAP_SHARED, configuracion->archivoDisco, 0);
	if(dataFile == (caddr_t) -1) {
		perror("mmap de read sector");
		exit(1);
	}
	memcpy(data, dataFile + (numeroSector * SECTOR_SIZE), SECTOR_SIZE);
	munmap(dataFile, SECTOR_SIZE);
}

void escribirSector(int32_t numeroSector, fat32_sector *data, struct configuracion *configuracion) {
	char *dataFile = mmap((caddr_t) 0, getpagesize(), PROT_WRITE, MAP_SHARED, configuracion->archivoDisco, 0);
	if(dataFile == (caddr_t) -1) {
		perror("mmap de write sector");
		exit(1);
	}
	memcpy(dataFile + (numeroSector * SECTOR_SIZE), data, SECTOR_SIZE);
	munmap(dataFile, SECTOR_SIZE);
}

int main(int argc, string *argv) {
	struct configuracion configuracion;
	configurar(argc, argv, &configuracion);
	abrirArchivoDisco(&configuracion);
    fat32_sector data;
	leerSector(0, &data, &configuracion);
	printf("Data: %s\n", data);
	leerSector(1, &data, &configuracion);
	printf("Sector1: %s\n", data);
	struct fat32_bootRecord bootRecord;
	memcpy(&bootRecord, data, sizeof data);
	printf("%d\n", bootRecord.serialNumber);
	//strcpy(data, "CHONEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
	//escribirSector(10, &data, &configuracion);
	leerSector(10, &data, &configuracion);
	printf("%s", data);
	return EXIT_SUCCESS;
}