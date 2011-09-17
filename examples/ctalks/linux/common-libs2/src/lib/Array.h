/*
 * @FILE: 		Array.h
 * @AUTHOR: 	vialef
 * @CREATED_ON: 15/03/2010
 * @VERSION:	
 */

#ifndef ARRAY_H_
#define ARRAY_H_

	#define _XOPEN_SOURCE 500

	#include <semaphore.h>

	#include "collections/Collections.h"
	#include "oop/Class.h"

	struct Array{
		const void * clazz;
		CollectionType contentType;
		void (*dataDestroyer)(void*);
		void **elements;
		int size;
		sem_t semaforo;
	};

	struct ArrayClass{
		void * (* get) (struct Array *, int index );
		void * (*set1)( struct Array *, int index, void* data );
		void   (*set2)( struct Array *, int index, void* data );
		void * (*find)( struct Array *, int (*closure)(void*) );
		void   (*iterator)( struct Array *, void (*closure)(void*) );
		void   (*clean)( struct Array * );
		int    (*size)( struct Array * );
	};

	struct Array	*Array_constructor( struct Array *, va_list *args );
	void 			*Array_get( struct Array *, int index );
	void			*Array_set1( struct Array *, int index, void* data );
	void		 	 Array_set2( struct Array *, int index, void* data);
	void			*Array_find( struct Array *, int (*closure)(void*) );
	void		 	 Array_iterator( struct Array *, void (*closure)(void*) );
	void		 	 Array_clean( struct Array * );
	int			 	 Array_size( struct Array * );
	void			*Array_destroyer( struct Array * );

	static const struct Class _Array = {
		"Array",
		sizeof(struct Array),
		(void*(*)(void*,va_list*))Array_constructor, Class_equals, (void*(*)(void*))Array_destroyer
	};

	static const void * Array = &_Array;

	static const struct ArrayClass ArrayClass = {
			Array_get,
			Array_set1,
			Array_set2,
			Array_find,
			Array_iterator,
			Array_clean,
			Array_size
	};

#endif /* ARRAY_H_ */
