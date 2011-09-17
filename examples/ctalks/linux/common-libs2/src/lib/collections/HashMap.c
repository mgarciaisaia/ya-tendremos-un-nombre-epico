
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#include "../oop/Object.h"
#include "HashMap.h"


/*static unsigned int HashMap_hash1(unsigned int key, int hash_size);*/
static unsigned int HashMap_hash1(char *key, int key_len);
static int HashMap_indexFor(struct HashMap *self, char *key);
static struct HashElement *Hash_getFreeElement( struct HashMap *self);
static void HashMap_resize(struct HashMap *self);

struct HashMap *HashMap_constructor( struct HashMap *self, va_list *args ){
	if( sem_init( &self->semaforo, 0, 1) == -1 ){
		return NULL;
	}
	self->contentType = va_arg(* args, CollectionType);
	if( self->contentType == POINTER_TYPE){
		self->dataDestroyer = va_arg(* args, void*);
	} else if( self->contentType != OBJECT_TYPE ){
		self->contentType = UNKWON_TYPE;
	}

	self->firstElementsBlock = new(ElementsBlock, DEFAULT_ELEMENTSBLOCK_SIZE, sizeof(struct HashElement));
	self->numberOfElementsBlocks = 1;

	self->elements = calloc(DEFAULT_ELEMENTSBLOCK_SIZE, sizeof( struct HashElement* ) );
	self->numberOfUsedElements = 0;

	self->maxsize = DEFAULT_ELEMENTSBLOCK_SIZE;
	self->size = 0;

	return self;
}
/*
static unsigned int HashMap_hash1(unsigned int key, int hash_size){
	// Robert Jenkins' 32 bit Mix Function
	key += (key << 12);
	key ^= (key >> 22);
	key += (key << 4);
	key ^= (key >> 9);
	key += (key << 10);
	key ^= (key >> 2);
	key += (key << 7);
	key ^= (key >> 12);

	// Knuth's Multiplicative Method
	key = (key >> 3) * 2654435761;

	return key % hash_size;
}
*/

static unsigned int HashMap_hash1(char *key, int key_len){
	unsigned int hash = 0;
	int index;

	for ( index = 0; index < key_len; index++ ){
		unsigned char c = key[index];
		hash += c;
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

static int HashMap_indexFor(struct HashMap *self, char *key){
	return  HashMap_hash1(key, strlen(key) ) % self->maxsize;
}

static void HashMap_resize(struct HashMap *self){
	struct HashElement **oldelements;
	int oldmaxsize;

	/* Backup all the elements */
	{
		oldelements = self->elements;
		oldmaxsize = self->maxsize;
	}

	/* Make a new block */
	{
		self->maxsize = self->maxsize * 2;
		self->elements = calloc( self->maxsize, sizeof( struct HashElement* ) );
	}

	/* Re insert all the elements*/
	{
		struct HashElement *auxelement;
		int index, newindex;
		for(index=0; index<oldmaxsize; index++){

			if( (auxelement=oldelements[index]) == NULL ) continue;

			while(auxelement != NULL){
				struct HashElement *nextelement = (struct HashElement*)auxelement->next;

				newindex = auxelement->hashcode % self->maxsize;
				auxelement->next = (void*)self->elements[newindex];
				self->elements[newindex] = auxelement;

				auxelement = nextelement;
			}

		}
	}

	/* Clean de old list */
	{
		free(oldelements);
	}

}

static struct HashElement *Hash_getFreeElement( struct HashMap *self){
	struct HashElement *freelement = NULL;
	struct ElementsBlock *pibotblock = self->firstElementsBlock;

	while( 1 ){
		freelement = ElementsBlockClass.getFreeElement(pibotblock);
		if( freelement != NULL ) break;
		if( pibotblock->next == NULL ) break;
		pibotblock = pibotblock->next;
	}

	if( freelement == NULL ){
		pibotblock->next = new(ElementsBlock, DEFAULT_ELEMENTSBLOCK_SIZE, sizeof(struct HashElement));
		self->numberOfElementsBlocks++;
		freelement = ElementsBlockClass.getFreeElement(pibotblock->next);
	}

	return freelement;
}

int HashMap_put( struct HashMap *self, char *key, void *data ){

	if( key == NULL ) return 0;

	sem_wait( &self->semaforo );
	{
		struct HashElement *element;
		int index;

		index = HashMap_indexFor(self, key);

		element = self->elements[index];

		if( element == NULL ){
			element = Hash_getFreeElement(self);
			element->hashcode = HashMap_hash1(key, strlen(key));
			element->key = malloc( strlen(key) + 1  );  strcpy(element->key, key);
			element->data = data;
			element->next = NULL;

			self->elements[index] = element;

			self->numberOfUsedElements++;
			self->size++;
		} else {

			for(; element != NULL; element = (void*)element->next){
				if( strcmp(element->key, key) == 0 ){
					if( self->contentType == POINTER_TYPE && self->dataDestroyer != NULL ){
						self->dataDestroyer(element->data);
					} else if (self->contentType == OBJECT_TYPE ){
						delete(element->data);
					}
					element->data = data;
					sem_post( &self->semaforo );
					return 1;
				}
			}

			element = Hash_getFreeElement(self);
			element->hashcode = HashMap_hash1(key, strlen(key));
			element->key = malloc( strlen(key) + 1  );  strcpy(element->key, key);
			element->data = data;
			element->next = (void*)self->elements[index];
			self->elements[index] = element;
			self->size++;
		}
	}

	if( self->numberOfUsedElements == self->maxsize ) HashMap_resize(self);

	sem_post( &self->semaforo );
	return 1;
}

void *HashMap_get( struct HashMap *self, char *key ){
	void* data = NULL;

	if( key == NULL ) return NULL;

	sem_wait( &self->semaforo );
	{
		int index = HashMap_indexFor(self, key);
		struct HashElement *element = self->elements[index];

		for(; element != NULL && strcmp(element->key, key) != 0 ; element = (void*)element->next);

		if( element == NULL ){
			sem_post( &self->semaforo );
			return NULL;
		}

		data = element->data;
	}
	sem_post( &self->semaforo );
	return data;
}

void HashMap_iterator( struct HashMap *self, void (*closure)(void*) ){

	sem_wait( &self->semaforo );
	{
		int index;
		for(index=0; index<self->maxsize; index++){
			struct HashElement *element = self->elements[index];
			for(;element != NULL; element=(void*)element->next){
				closure(element->data);
			}
		}
	}
	sem_post( &self->semaforo );
}

void *HashMap_remove1( struct HashMap *self, char *key ){
	void *data = NULL;

	sem_wait( &self->semaforo );
	{
		int index = HashMap_indexFor(self, key);
		struct HashElement *element = self->elements[index];
		struct HashElement *auxelement = NULL;

		while( element != NULL ){
			if( strcmp(element->key, key) == 0 ) {

				if( auxelement == NULL  ){
					self->elements[index] = (void*)element->next;
				} else {
					auxelement->next = element->next;
				}

				break;
			}

			auxelement = element;
			element = (void*)element->next;
		}

		if( element == NULL ){
			sem_post( &self->semaforo );
			return NULL;
		}

		data = element->data;

		element->inuse = 0;
		free(element->key);
		element->key = NULL;
		element->data = NULL;
		element->next = NULL;

		((struct ElementsBlock*)element->block)->elements_free++;

		if( self->elements[index] == NULL ) self->numberOfUsedElements--;
		self->size--;

	}
	sem_post( &self->semaforo );

	return data;
}

void HashMap_remove2( struct HashMap *self, char *key ){
	void *data = HashMap_remove1(self, key);

	if( data == NULL ) return;

	if( self->contentType == POINTER_TYPE && self->dataDestroyer != NULL ){
		self->dataDestroyer( data );
	} else if ( self->contentType == OBJECT_TYPE ){
		delete( data );
	}
}

int HashMap_isEmpty( struct HashMap *self ){
	return self->size == 0;
}

int	HashMap_size( struct HashMap *self){
	return self->size;
}

void HashMap_clean( struct HashMap *self ){

	sem_wait( &self->semaforo );
	{
		int index;
		for(index=0; index<self->maxsize; index++){
			struct HashElement *element = self->elements[index];
			void *data;

			while( element != NULL ){
				struct HashElement *nextelement = (void*)element->next;
				struct ElementsBlock *block = element->block;
				element->inuse = 0;

				free(element->key);
				element->key = NULL;

				data = element->data;
				element->data = NULL;

				element->next = NULL;
				block->elements_free++;

				if( self->contentType == POINTER_TYPE && self->dataDestroyer != NULL ){
					self->dataDestroyer( data );
				} else if ( self->contentType == OBJECT_TYPE ){
					delete( data );
				}

				element = nextelement;
			}

			self->elements[index] = NULL;
		}
		self->numberOfUsedElements = 0;
		self->size = 0;
	}
	sem_post( &self->semaforo );
}


void *HashMap_destroyer( struct HashMap *self ){
	HashMap_clean( self );
	sem_destroy( &self->semaforo );
	free( self->elements );
	delete( self->firstElementsBlock );
	return self;
}

