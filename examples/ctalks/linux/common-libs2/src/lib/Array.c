#include <stdlib.h>
#include <string.h>

#include "oop/Object.h"
#include "Array.h"

struct Array	*Array_constructor( struct Array *self, va_list *args ){
	if( sem_init( &self->semaforo, 0, 1) == -1 ){
		return NULL;
	}
	self->contentType = va_arg(* args, CollectionType);
	if( self->contentType == POINTER_TYPE){
		self->dataDestroyer = va_arg(* args, void*);
	} else if( self->contentType != OBJECT_TYPE ){
		self->contentType = UNKWON_TYPE;
	}

	self->size = va_arg(* args, int);

	self->elements = calloc(self->size, sizeof(void*) );

	return self;
}

void	*Array_get( struct Array *self, int index ){
	void *data = NULL;

	sem_wait( &self->semaforo );
	{
		if( index >= 0 || index < self->size ){
			data = self->elements[index];
		}
	}
	sem_post( &self->semaforo );
	return data;
}

void	*Array_set1( struct Array *self, int index, void* data ){
	void *olddata = NULL;

	sem_wait( &self->semaforo );
	{
		if( index >= 0 || index < self->size ){
			olddata = self->elements[index];
			self->elements[index] = data;
		}
	}
	sem_post( &self->semaforo );
	return olddata;
}

void	Array_set2( struct Array *self, int index, void* data){
	void *olddata = Array_set1(self, index, data);

	if( self->contentType == POINTER_TYPE && self->dataDestroyer != NULL ){
		self->dataDestroyer( olddata );
	} else if (self->contentType == OBJECT_TYPE ){
		delete( olddata );
	}

}

void	*Array_find( struct Array *self, int (*closure)(void*) ){
	void *data = NULL;
	sem_wait( &self->semaforo );
	{
		int index;
		for(index=0; index<self->size; index++){
			if( closure(self->elements[index]) ){
				data = self->elements[index];
				break;
			}
		}
	}
	sem_post( &self->semaforo );
	return data;
}

void 	Array_iterator( struct Array *self, void (*closure)(void*) ){
	sem_wait( &self->semaforo );
	{
		int index;
		for(index=0; index<self->size; index++){
			closure(self->elements[index]);
		}
	}
	sem_post( &self->semaforo );
}

void	Array_clean( struct Array *self ){
	sem_wait( &self->semaforo );
	{
		int index;
		for(index=0; index<self->size; index++){

			if( self->contentType == POINTER_TYPE && self->dataDestroyer != NULL ){
				self->dataDestroyer( self->elements[index] );
			} else if (self->contentType == OBJECT_TYPE ){
				delete( self->elements[index] );
			}

			self->elements[index] = NULL;
		}
	}
	sem_post( &self->semaforo );
}

int		Array_size( struct Array *self ){
	return self->size;
}

void   *Array_destroyer( struct Array *self ){
	Array_clean( self );
	sem_destroy( &self->semaforo );
	free( self->elements );
	return self;
}
