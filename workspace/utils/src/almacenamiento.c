/*
 * almacenamiento.c
 *
 *  Created on: 22/10/2011
 *      Author: choni
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "almacenamiento.h"
#include "configuracion.h"
#include "log.h"



static const int SECTOR_SIZE = 512;

void abrirArchivoDisco(configuracion *configuracion) {
	configuracion->archivoDisco = open(configuracion->rutaAlDisco, O_RDWR);
	struct stat statsDisco;
	fstat(configuracion->archivoDisco, &statsDisco);
	configuracion->tamanioDisco = statsDisco.st_size;
	perror("abrirARchivo");
}

void leerSector(int32_t numeroSector, fat32_sector *data, configuracion *configuracion) {
	char *dataFile = mmap((caddr_t) 0, configuracion->tamanioDisco, PROT_READ, MAP_SHARED, configuracion->archivoDisco, 0);
	if(dataFile == (caddr_t) -1) {
		perror("mmap de read sector");
		exit(1);
	}
	memcpy(data, dataFile + (numeroSector * SECTOR_SIZE), SECTOR_SIZE);
	munmap(dataFile, configuracion->tamanioDisco);
}

void escribirSector(int32_t numeroSector, fat32_sector *data, configuracion *configuracion) {
	char *dataFile = mmap((caddr_t) 0, configuracion->tamanioDisco, PROT_WRITE, MAP_SHARED, configuracion->archivoDisco, 0);
	if(dataFile == (caddr_t) -1) {
		perror("mmap de write sector");
		exit(1);
	}
	memcpy(dataFile + (numeroSector * SECTOR_SIZE), data, SECTOR_SIZE);
	munmap(dataFile, configuracion->tamanioDisco);
}

