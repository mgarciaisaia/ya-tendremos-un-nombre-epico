#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "NString.h"

static void NString_trimBounds( char *str, int *beginIndex, int *newStrLen );

int	NString_startWith( char *str, char *prefix ){
	return strncmp(str, prefix, strlen(prefix)) == 0;
}

int	NString_asInt( char *str ){
	int num = atoi(str);
	return num;
}

double	NString_asDouble( char *str ){
	double num = atof(str);
	return num;
}

int	NString_length( char *str ){
	return strlen(str);
}

char**	NString_split( char *str, char delimitor ){
	char **array;
	int elementsAmount = 0;

	{
		int cont;
		for(cont=0; str[cont] != '\0'; cont++ ){
			if( str[cont] == delimitor)	elementsAmount++;
		}
		elementsAmount++;
	}

	{
		array = calloc(elementsAmount+1, sizeof(char*) );
	}

	{
		int pibot, length, cont, elementIndex;
		for(cont=0, pibot=0, length=0, elementIndex=0; elementIndex != elementsAmount ; cont++ ){
			if( str[cont] == delimitor || str[cont] == '\0' ){
				array[elementIndex] = malloc(length + 1);
				if( length > 0 ){
					strncpy(array[elementIndex], &str[pibot], length);
					array[elementIndex][length] = '\0';
				} else {
					array[elementIndex][0] = '\0';
				}
				pibot = pibot + length + 1;
				length = 0;
				elementIndex++;
			} else {
				length++;
			}
		}
		array[elementIndex] = NULL;
	}
	return array;
}

static void NString_trimBounds( char *str, int *beginIndex, int *newStrLen ){
	int endIndex,index;
	int strLen = strlen(str);

	if( strLen <= 0 ){
		*beginIndex = 0;
		*newStrLen = 0;
		return;
	}

	for(index=0 ;isspace(str[index]) || str[index] == '\n'; index++);		*beginIndex=index;
	for(index=strLen-1 ;isspace(str[index]) || str[index] == '\n'; index--);	endIndex=index;

	/*	Se le suma +1 porque porque endIndex y beginIndex
		son indices y el len es por cantidad de elementos	*/
	*newStrLen = endIndex - *beginIndex + 1;
}

char*	NString_trim( char *str ){
	char* newStr;
	int beginIndex, newStrLen;

	NString_trimBounds(str, &beginIndex, &newStrLen);

	if( newStrLen == 0 ) return "";

	/*	Se le suma +1 por el \0	*/
	newStr = malloc( newStrLen + 1);

	memcpy(newStr, &str[beginIndex], newStrLen);

	newStr[newStrLen]='\0';

	return newStr;
}

char *NString_trimInBuffer( char *str, char *buffer ){
	int beginIndex,newStrLen;

	NString_trimBounds(str, &beginIndex, &newStrLen);

	if( newStrLen == 0 ) return "";

	memcpy(buffer, &str[beginIndex], newStrLen);

	buffer[newStrLen]='\0';

	return buffer;
}

char* NString_clone( char *str ){
	char *newStr = malloc( strlen(str) + 1 );
	strcpy(newStr, str);
	return newStr;
}

int	NString_indexOf1( char *str1, char *str2){
	return NString_indexOf2(str1, 0, str2);
}

int	NString_indexOf2( char *str1, int offset, char *str2){
	char * found = strstr( str1 + offset, str2 );
	int index = found - str1;

	if( index < 0 ) return -1;

	return index;
}

int NString_equals( char *str1, char *str2 ){
	return strcmp(str1, str2) == 0;
}
