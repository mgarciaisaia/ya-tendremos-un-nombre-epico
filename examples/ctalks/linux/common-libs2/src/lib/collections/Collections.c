
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "../oop/Object.h"

#include "Collections.h"

struct ElementsBlock *ElementsBlock_constructor( struct ElementsBlock *self, va_list *args ){
	self->size = va_arg(* args, int);
	assert( self->size );
	self->element_size = va_arg(* args, int);
	assert( self->element_size );
	self->elements_free = self->size;
	self->elements = calloc(self->size, self->element_size );
	return self;
}

void *ElementsBlock_getFreeElement( struct ElementsBlock * self){
	struct CollectionElement *freeElement;

	if( self->elements_free == 0 )	return NULL;

	{
		int index;
		for(index=0; index<self->size ;index++ ){
			freeElement = (struct CollectionElement*)&self->elements[index*self->element_size];
			if( freeElement->inuse == 0 )	break;
		}
	}

	freeElement->block = self;
	freeElement->inuse = 1;
	self->elements_free--;
	return (void*)freeElement;
}

void *ElementsBlock_get( struct ElementsBlock * self, int index ){
	if( index < 0 ) return NULL;
	if( self->next == NULL && index >= self->size ) return NULL;
	if( self->next != NULL && index >= self->size ) return ElementsBlock_get(self->next, index - self->size);

	struct CollectionElement *element = (struct CollectionElement*)&self->elements[index*self->element_size];
	element->block = self;

	return element;
}

int	ElementsBlock_size( struct ElementsBlock * self){
	return self->size;
}

int ElementsBlock_totalSize( struct ElementsBlock * self){
	return self->size + self->next != NULL ? ElementsBlock_totalSize(self->next) : 0;
}

void *ElementsBlock_destroyer( struct ElementsBlock * self){
	if( self->next != NULL ){
		delete( self->next );
		self->next = NULL;
	}
	free( self->elements );
	return self;
}
