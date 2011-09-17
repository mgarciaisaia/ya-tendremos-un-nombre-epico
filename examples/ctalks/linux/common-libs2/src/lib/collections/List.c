
#include <stdlib.h>
#include <semaphore.h>

#include "../oop/Object.h"
#include "List.h"


static struct DoubleLinkedElement *List_getFreeElement( struct List *);

struct List	*List_constructor( struct List *self, va_list *args ){
	if( sem_init( &self->semaforo, 0, 1) == -1 ){
		return NULL;
	}
	self->contentType = va_arg(* args, CollectionType);
	if( self->contentType == POINTER_TYPE){
		self->dataDestroyer = va_arg(* args, void*);
	} else if( self->contentType != OBJECT_TYPE ){
		self->contentType = UNKWON_TYPE;
	}
	self->size = 0;
	self->firstBlock = new(ElementsBlock, DEFAULT_ELEMENTSBLOCK_SIZE, sizeof(struct DoubleLinkedElement) );
	self->numberOfBlocks = 1;
	return self;
}

static struct DoubleLinkedElement *List_getFreeElement( struct List *self){
	struct DoubleLinkedElement *freelement = NULL;
	struct ElementsBlock *pibotblock = self->firstBlock;

	while( 1 ){
		freelement = ElementsBlockClass.getFreeElement(pibotblock);
		if( freelement != NULL ) break;
		if( pibotblock->next == NULL ) break;
		pibotblock = pibotblock->next;
	}

	if( freelement == NULL ){
		pibotblock->next = new(ElementsBlock, pibotblock->size * 2 , sizeof(struct DoubleLinkedElement));
		self->numberOfBlocks++;
		freelement = ElementsBlockClass.getFreeElement(pibotblock->next);
	}

	return freelement;
}


/*
 * @NAME: collection_list_add
 * @DESC: Agrega un elemento al final de la lista
 */

int List_add( struct List *self, void *data ){
	sem_wait( &self->semaforo );
	{
		struct DoubleLinkedElement *freeElement = List_getFreeElement(self);
		freeElement->data = data;
		if( self->lastElement == NULL ){
			self->firstElement = freeElement;
			self->lastElement = freeElement;
		} else {
			self->lastElement->next = (void*)freeElement;
			freeElement->previous = self->lastElement;
			self->lastElement = freeElement;
		}
		self->size++;
	}
	sem_post( &self->semaforo );
	return self->size - 1;
}

void *List_get( struct List *self, int index ){
	void* data = NULL;

	if( index < 0 || index >= self->size ) return NULL;

	sem_wait( &self->semaforo );
	{
		struct DoubleLinkedElement *element;
		if( index <= self->size / 2 ){
			element = (void*)self->firstElement;
			int pibot;
			for( pibot=0; pibot<index; element=(void*)element->next,pibot++ );
		} else {
			element = (void*)self->lastElement;
			int pibot;
			for( pibot=self->size-1; pibot>index; element=(void*)element->previous,pibot-- );
		}
		data = element->data;
	}
	sem_post( &self->semaforo );
	return data;
}


void List_put( struct List *self, int index, void *data ){
	if( index < 0 || index >= self->size ) return;

	sem_wait( &self->semaforo );
	{
		struct DoubleLinkedElement *freeElement = List_getFreeElement(self);
		freeElement->data = data;
		if( index == 0){
			freeElement->next = (void*)self->firstElement;
			self->firstElement = freeElement;
		} else if( index == self->size - 1 ){
			freeElement->next = (void*)self->lastElement;
			freeElement->previous = self->lastElement->previous;
			self->lastElement->previous->next = (void*)freeElement;
			self->lastElement = freeElement;
		} else {
			struct DoubleLinkedElement *pibotElement = self->firstElement;
			int cont;
			for( cont=0; cont<index; pibotElement=(void*)pibotElement->next,cont++ );
			freeElement->next = (void*)pibotElement;
			freeElement->previous = pibotElement->previous;
			pibotElement->previous->next = (void*)freeElement;
		}
	}
	sem_post( &self->semaforo );
}

void *List_set1( struct List *self, int index, void* data ){
	void *oldData = NULL;

	if( index < 0 || index >= self->size ) return NULL;

	sem_wait( &self->semaforo );
	{
		struct DoubleLinkedElement *pibotElement = self->firstElement;
		int cont;
		for( cont=0; cont<index; pibotElement=(void*)pibotElement->next,cont++ );
		oldData = pibotElement->data;
		pibotElement->data = data;
	}
	sem_post( &self->semaforo );
	return oldData;
}

void List_set2( struct List *self, int index, void* data ){
	void *aux = List_set1(self, index, data);

	if( aux == NULL) return;

	if( self->contentType == POINTER_TYPE && self->dataDestroyer != NULL ){
		self->dataDestroyer(aux);
	} else if (self->contentType == OBJECT_TYPE ){
		delete(data);
	}

}


/*
 * @NAME: collection_list_find
 * @DESC: Retorna el primer valor encontrado, el cual haga que el closure devuelva != 0
 */

void *List_find( struct List *self, int (*closure)(void*) ){
	void *data = NULL;

	sem_wait( &self->semaforo );
	{
		struct DoubleLinkedElement *pibotElement = self->firstElement;
		while( pibotElement != NULL ){
			if( closure(pibotElement->data) ){
				data = pibotElement->data;
				break;
			}
			pibotElement = (void*)pibotElement->next;
		}
	}
	sem_post( &self->semaforo );
	return data;
}

void List_iterator( struct List *self, void (*closure)(void*) ){

	sem_wait( &self->semaforo );
	{
		struct DoubleLinkedElement *pibotElement = self->firstElement;
		while( pibotElement != NULL ){
			closure(pibotElement->data);
			pibotElement = (void*)pibotElement->next;
		}
	}
	sem_post( &self->semaforo );
}

void *List_remove1( struct List *self, int index ){
	void *data = NULL;

	if( index < 0 || index >= self->size ) return NULL;

	sem_wait( &self->semaforo );
	{
		if( index == 0 ){
			struct DoubleLinkedElement *auxElement = self->firstElement;
			struct ElementsBlock *auxblock = auxElement->block;

			self->firstElement = (void*)self->firstElement->next;
			if( self->size > 1) self->firstElement->previous = NULL;
			if( self->size == 1) self->lastElement = NULL;

			auxElement->inuse = 0;
			auxElement->next = NULL;
			auxElement->previous = NULL;
			data = auxElement->data;
			auxElement->data = NULL;
			auxblock->elements_free++;

		}else if( (self->size > index) && (index > 0) ){
			struct DoubleLinkedElement *pibotElement = self->firstElement;
			int cont;
			for( cont=0; cont<index; pibotElement=(void*)pibotElement->next, cont++ );

			if( self->lastElement == pibotElement) {
				self->lastElement = pibotElement->previous;
				self->lastElement->next = NULL;
			} else {
				struct DoubleLinkedElement *auxElement = (void*)pibotElement->next;
				pibotElement->previous->next = (void*)auxElement;
				auxElement->previous = pibotElement->previous;
			}

			data = pibotElement->data;
			pibotElement->inuse = 0;
			pibotElement->next = NULL;
			pibotElement->previous = NULL;
			pibotElement->data = NULL;
			((struct ElementsBlock*)pibotElement->block)->elements_free++;
		}
		self->size--;
	}
	sem_post( &self->semaforo );

	return data;
}

void List_remove2( struct List *self, int index ){
	void *data = List_remove1(self, index);

	if( data == NULL ) return;

	if( self->contentType == POINTER_TYPE && self->dataDestroyer != NULL ){
		self->dataDestroyer( data );
	} else if ( self->contentType == OBJECT_TYPE ){
		delete( data );
	}
}

void List_removeByClosure1( struct List *self, int (*closure)(void*) ){
	sem_wait( &self->semaforo );
	{
		struct DoubleLinkedElement *pibotElement = self->firstElement;
		while( pibotElement != NULL ){

			if( closure(pibotElement->data) != 0 ){
				struct DoubleLinkedElement *auxElement = pibotElement;

				if( auxElement == self->firstElement ){
					self->firstElement = (void*)self->firstElement->next;
					if( self->size > 1) self->firstElement->previous = NULL;
					if( self->size == 1) self->lastElement = NULL;
					pibotElement = self->firstElement;
				} else if( auxElement == self->lastElement ) {
					self->lastElement = self->lastElement->previous;
					self->lastElement->next = NULL;
					pibotElement = NULL;
				} else {
					pibotElement->previous->next = pibotElement->next;
					((struct DoubleLinkedElement*)pibotElement->next)->previous = pibotElement->previous;
					pibotElement = (void*)pibotElement->next;
				}

				auxElement->inuse = 0;
				auxElement->next = NULL;
				auxElement->previous = NULL;
				auxElement->data = NULL;
				((struct ElementsBlock*)auxElement->block)->elements_free++;
				self->size--;
			} else {
				pibotElement = (void*)pibotElement->next;
			}
		}
	}
	sem_post( &self->semaforo );
}

void List_removeByClosure2( struct List *self, int (*closure)(void*) ){
	sem_wait( &self->semaforo );
	{
		struct DoubleLinkedElement *pibotElement = self->firstElement;
		while( pibotElement != NULL ){

			if( closure(pibotElement->data) != 0 ){
				struct DoubleLinkedElement *auxElement = pibotElement;

				if( auxElement == self->firstElement ){
					self->firstElement = (void*)self->firstElement->next;
					if( self->size > 1) self->firstElement->previous = NULL;
					if( self->size == 1) self->lastElement = NULL;
					pibotElement = self->firstElement;
				} else if( auxElement == self->lastElement ) {
					self->lastElement = self->lastElement->previous;
					self->lastElement->next = NULL;
					pibotElement = NULL;
				} else {
					pibotElement->previous->next = pibotElement->next;
					((struct DoubleLinkedElement*)pibotElement->next)->previous = pibotElement->previous;
					pibotElement = (void*)pibotElement->next;
				}

				auxElement->inuse = 0;
				auxElement->next = NULL;
				auxElement->previous = NULL;

				if( self->contentType == POINTER_TYPE && self->dataDestroyer != NULL ){
					self->dataDestroyer(auxElement->data);
				} else if (self->contentType == OBJECT_TYPE ){
					delete(auxElement->data);
				}

				auxElement->data = NULL;
				((struct ElementsBlock*)auxElement->block)->elements_free++;
				self->size--;
			} else {
				pibotElement = (void*)pibotElement->next;
			}
		}
	}
	sem_post( &self->semaforo );
}

void List_removeByPointer(  struct List *self, void *data ){
	int inner_finderByPointer(void *element){
		return element == data;
	}
	List_removeByClosure2(self, inner_finderByPointer);
}


int List_isEmpty( struct List *self ){
	return self->size == 0;
}

int	List_size( struct List *self){
	return self->size;
}

void List_clean( struct List *self ){
	struct DoubleLinkedElement *pibotElement;

	sem_wait( &self->semaforo );
	{
		while( self->firstElement != NULL ){
			pibotElement = self->firstElement;
			self->firstElement = (void*)self->firstElement->next;
			pibotElement->inuse = 0;
			pibotElement->next = NULL;
			pibotElement->previous = NULL;

			if( self->contentType == POINTER_TYPE && self->dataDestroyer != NULL ){
				self->dataDestroyer(pibotElement->data);
			} else if (self->contentType == OBJECT_TYPE ){
				delete(pibotElement->data);
			}

			pibotElement->data = NULL;
		}
		self->lastElement = NULL;
		self->size = 0;

		if(  self->firstBlock->next != NULL ){
			delete( self->firstBlock->next );
			self->firstBlock->next = NULL;
		}
		self->firstBlock->elements_free = self->firstBlock->size;
		self->numberOfBlocks = 1;
	}
	sem_post( &self->semaforo );
}


void *List_destroyer( struct List *self ){
	List_clean( self );
	sem_destroy( &self->semaforo );
	delete( self->firstBlock );
	return self;
}

