#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "File.h"

static char* File_load(struct File *self);

struct File* File_constructor( struct File *self, va_list *args ){
	char* path = va_arg(* args, char*);

	self->path = malloc( strlen(path) + 1 );
	strcpy(self->path, path);

	return self;
}

static char* File_load(struct File *self){
	int file_size = File_size(self);

	if( ( self->desc=fopen(self->path,"r") ) == NULL ){
		return NULL;
	}

	self->text = malloc( file_size + 1 );

	{
		int index, lineLenght = 0;
		for(index=0; index<file_size; index++){
			self->text[index] = fgetc(self->desc);
			if( self->text[index] == '\n' || self->text[index] == '\0'){
				if( self->maxLineLength < lineLenght){
					self->maxLineLength = lineLenght;
					lineLenght = 0;
				}
				self->numberOfLines++;
			} else {
				lineLenght++;
			}
		}
		self->numberOfLines++;
		self->text[index]='\0';
	}
	return self->text;
}


int	File_exist( struct File *self){
	struct stat st;
	if(stat(self->path, &st) == 0)	return 1;
	return 0;
}

int	File_isDirectory( char *path ){
	int status;
	struct stat astatBuf;
	status = stat(path, &astatBuf);
	status = (status == 0 && S_ISDIR(astatBuf.st_mode));
	return status;
}

int	File_create( struct File *self ){
	self->desc = fopen(self->path,"w+");
	if (self->desc == NULL) return 0;
	return 1;
}

int	File_delete( struct File *self ){
	if( remove( self->path ) == -1 ) return 0;
	return 1;
}

int	File_size( struct File *self ){
	struct stat file;
	if( stat(self->path, &file) ) return FILE_NOT_FOUND;
	return file.st_size;
}

char* File_read( struct File *self ){
	if( self->text != NULL ) free(self->text);
	return File_load(self);
}

void File_iterateLines( struct File *self, void (*closure)(char* line) ){
	char buffer[ self->maxLineLength ];
	int index, bufferindex;
	char c;

	if( self->text == NULL ) return;

	for(index=0, bufferindex=0; (c=self->text[index]) != '\0'; index++){
		if( c != '\n' ){
			buffer[bufferindex] = c;
			bufferindex++;
		} else {
			buffer[bufferindex] = '\0';
			bufferindex=0;
			closure(buffer);
			if( self->text[index + 1] == '\r' ) index++;
		}
	}
	buffer[bufferindex] = c;
	closure(buffer);
}

int	File_write( struct File *self ){
	int index;

	if(self->desc == NULL) {
		if( ( self->desc=fopen(self->path,"ab+") ) == NULL) return 0;
	}

	for(index=0; index < strlen(self->text) ;index++){
		fputc( self->text[index], self->desc);
	}

	fflush(self->desc);
	return 1;
}

void* File_destroyer( struct File *self ){
	if( self->desc != NULL ) fclose(self->desc);
	if( self->text != NULL ) free( self->text );
	free( self->path );
	return self;
}
