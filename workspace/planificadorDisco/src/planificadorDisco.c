#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "src/almacenamiento.h"
#include "src/configuracion.h"
#include "src/log.h"

static const char *RUTA_ARCHIVO_CONFIGURACION = "../conf/planificador.ini";
static const int SECTOR_SIZE = 512;
//typedef char* string;

struct s_configuracion {
	string rutaAlDisco;
	int archivoDisco;
	off_t tamanioDisco;
	string modo;
	t_log logger;
};
typedef struct s_configuracion configuracion;

struct fat32_bootRecord {
	char jumpInstruction[3];
	char oemName[8];
	int16_t sectorSize;
	char clusterSize;
	int16_t fatOffset; // Sectores desde el inicio de este sector hasta la primer FAT (mínimo 1: el boot sector)
	char fatsCount; // Cantidad de FATs
	int16_t uselessField1;
	int16_t sectorCountSmall; // Cantidad de sectores del disco, o 0 si son mas de 64k
	char deviceType;
	int16_t uselessField2;
	int16_t trackSectors; // Sectores por pista (S de CHS)
	int16_t heads; // H de CHS
	int32_t bootRecordOffset;
	int32_t sectorCountLarge; // Cantidad de sectores del disco si son mas de 64k, ver sectorCountSmall sino
	int32_t fatSize; // Sectores por FAT
	int16_t fatConversionFlags; // Flags de la conversion FAT12/16 => FAT32
	int16_t fatVersion; // 0 = FAT32
	int32_t rootDirectory;
	int16_t fileSystemInfoSector;
	int16_t bootSectorBackup;
	char uselessField3[12];
	char physicalDeviceType; // 0x00 = removible; 0x80 = disco rigido
	char uselessField4;
	char extendedBootSignature;
	int32_t serialNumber;
	char volumeLabel[11];
	char fatType[8]; // Siempre "FAT32   "
	char bootCode[420]; // "NO LE INTERESA A NADIE!!" :)
	char bootSectorSignature[2]; // Siempre 0x55 0xAA
}__attribute__ ((__packed__));


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

void crearLogger(configuracion *configuracion) {
	FILE * archivoConfiguracion = fopen(RUTA_ARCHIVO_CONFIGURACION, "r");
	string archivoLogger = valorConfiguracion(archivoConfiguracion, "logger.rutaArchivo");
	e_console_mode modoConsola = M_CONSOLE_DISABLE;
	// yupi! strcmp devuelve 0 (a.k.a. FALSE) cuando COINCIDEN las cadenas
	if(!strcmp("1", valorConfiguracion(archivoConfiguracion, "logger.habilitarConsola"))){
		modoConsola = M_CONSOLE_ENABLE;
	}
	//e_message_level nivelLog = DEBUG;
	// TODO: perdon
	char logLeido = tolower(valorConfiguracion(archivoConfiguracion, "logger.habilitarConsola")[0]);
	fclose(archivoConfiguracion);
	log_create("planificadorDisco", archivoLogger, logLeido, modoConsola);
}

/**
 * Levanta las configuraciones, abre el archivo de disco, etc
 *
 * Deja todo preparado para poder operar
 */
void configurar(int argc, string argv[], configuracion *configuracion) {
	crearLogger(configuracion);
	if(argc != 2) {
		salirConModoDeUso(argc, argv);
	}

	configuracion->rutaAlDisco = argv[1];
	FILE * archivoConfiguracion = fopen(RUTA_ARCHIVO_CONFIGURACION, "r");
	configuracion->modo = valorConfiguracion(archivoConfiguracion, "planificador.modo");

	// TODO: aca deberia leer el planificador.ini y seguir leyendo configuracion
}

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

int main(int argc, string *argv) {
	configuracion configuracion;
	configurar(argc, argv, &configuracion);
	abrirArchivoDisco(&configuracion);
    fat32_sector data;
	leerSector(10, &data, &configuracion);
	printf("Data: %s\n", data);
	leerSector(0, &data, &configuracion);
	printf("Sector1: %s\n", data);
	printf("PAGESIZE: %d\n%d\n", getpagesize(), getpagesize() / SECTOR_SIZE);
	strcpy(data, "VA EL 10");
	escribirSector(10, &data, &configuracion);
	strcpy(data, "                                                   ");
	printf("%s", data);
	leerSector(10, &data, &configuracion);
	printf("%s", data);
	close(configuracion.archivoDisco);
	return EXIT_SUCCESS;
}
