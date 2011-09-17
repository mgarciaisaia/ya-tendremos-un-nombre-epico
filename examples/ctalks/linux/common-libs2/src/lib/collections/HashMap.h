

#ifndef HASHMAP_H_
#define HASHMAP_H_

	#include <semaphore.h>

	#include "Collections.h"
	#include "../oop/Class.h"

	struct HashMap {
		const void * clazz;
		CollectionType contentType;
		void (*dataDestroyer)(void*);
		struct HashElement **elements;
		int numberOfUsedElements;
		struct ElementsBlock *firstElementsBlock;
		int numberOfElementsBlocks;
		int maxsize;
		int size;
		sem_t semaforo;
	};

	struct HashMapClass {
		int   (*put)( struct HashMap *, char *key, void *data);
		void *(*get)( struct HashMap *, char *key );
		void *(*remove1)( struct HashMap *, char *key );
		void  (*remove2)( struct HashMap *, char *key );
		void  (*iterator)( struct HashMap *, void (*closure)(void*) );
		void  (*clean)( struct HashMap * );
		int   (*isEmpty)( struct HashMap * );
		int   (*size)( struct HashMap * );
	};

	struct HashMap *HashMap_constructor( struct HashMap *, va_list *args );
	int 			HashMap_put( struct HashMap *, char *key, void *data );
	void		   *HashMap_get( struct HashMap *, char *key );
	void	  	   *HashMap_remove1( struct HashMap *, char *key );
	void			HashMap_remove2( struct HashMap *, char *key );
	void			HashMap_iterator( struct HashMap *, void (*closure)(void*) );
	void			HashMap_clean( struct HashMap * );
	int				HashMap_isEmpty( struct HashMap * );
	int				HashMap_size( struct HashMap * );
	void		   *HashMap_destroyer( struct HashMap * );

	static const struct Class _HashMap = {
		"HashMap",
		sizeof(struct HashMap),
		(void*(*)(void*,va_list*))HashMap_constructor, Class_equals, (void*(*)(void*))HashMap_destroyer
	};

	static const void * HashMap = &_HashMap;

	static const struct HashMapClass HashMapClass = {
			HashMap_put,
			HashMap_get,
			HashMap_remove1,
			HashMap_remove2,
			HashMap_iterator,
			HashMap_clean,
			HashMap_isEmpty,
			HashMap_size
	};

#endif /* HASHMAP_H_ */

