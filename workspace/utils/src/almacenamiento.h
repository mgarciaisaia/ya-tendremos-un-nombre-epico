#ifndef ALMACENAMIENTO_H_
#define ALMACENAMIENTO_H_

/*
 * En almacenamiento.h debieramos poner la interfaz comun de los procesos que
 * tratan con el almacenamiento fisico de los datos, es decir, el RAID y el
 * disco
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


struct s_configuracion {
	string rutaAlDisco;
	int archivoDisco;
	off_t tamanioDisco;
	string modo;
	t_log *logger;
};
typedef struct s_configuracion configuracion;

typedef char fat32_sector[512];

void abrirArchivoDisco(configuracion *configuracion);

void leerSector(int32_t numeroSector, fat32_sector *data, configuracion *configuracion);

void escribirSector(int32_t numeroSector, fat32_sector *data, configuracion *configuracion);


#endif /* ALMACENAMIENTO_H_ */
