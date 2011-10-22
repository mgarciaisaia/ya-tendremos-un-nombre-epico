#include <stdio.h>
#include <stdlib.h>
#include "configuracion.h"

int main(int argc, char *argv[]) {
	FILE * archivoConfiguracion = fopen("/home/desert69/operativos/workspace/planificadorDisco/conf/planificador.ini", "r");
	valorConfiguracion(archivoConfiguracion, "planificador.modo");
	valorConfiguracion(archivoConfiguracion, "planificador.cabezas");
	valorConfiguracion(archivoConfiguracion, "planificador.modoQUENOEXISTE");
	valorConfiguracion(archivoConfiguracion, "planificador.idDisco");
	return EXIT_SUCCESS;
}