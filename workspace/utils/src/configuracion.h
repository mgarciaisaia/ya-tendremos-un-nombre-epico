#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
typedef char* string;
typedef char boolean;

string valorConfiguracion(FILE * archivo, string clave);

#endif /* CONFIGURACION_H_ */