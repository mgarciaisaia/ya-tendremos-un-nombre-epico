

#ifndef LIST_H_
#define LIST_H_

	#include <semaphore.h>

	#include "Collections.h"
	#include "../oop/Class.h"

	struct List {
	#define List_DEF 								\
		const void * clazz; 						\
		struct DoubleLinkedElement *firstElement;	\
		struct DoubleLinkedElement *lastElement;	\
		struct ElementsBlock *firstBlock;			\
		CollectionType contentType;					\
		void (*dataDestroyer)(void*);				\
		int numberOfBlocks;							\
		int size;									\
		sem_t semaforo;
	List_DEF
	};

	struct ListClass {
		int (* add) (struct List *, void *data );
		void * (* get) (struct List *, int index );
		void (* put) ( struct List *, int index, void *data );
		void * (*set1)( struct List *, int index, void* data );
		void (*set2)( struct List *, int index, void* data );
		void * (*find)( struct List *, int (*closure)(void*) );
		void (*iterator)( struct List *, void (*closure)(void*) );
		void * (*remove1)( struct List *, int index);
		void (*remove2)( struct List *, int index );
		void (*removeByPointer)( struct List *, void *data );
		void (*removeByClosure1)( struct List *, int (*closure)(void*) );
		void (*removeByClosure2)( struct List *, int (*closure)(void*) );
		void (*clean)( struct List * );
		int (*isEmpty)( struct List * );
		int (*size)( struct List * );
	};

	struct List	*List_constructor( struct List *, va_list *args );
	int			 List_add( struct List *, void *data );
	void 		*List_get( struct List *, int index );
	void		 List_put( struct List *, int index, void *data );
	void		*List_set1( struct List *, int index, void* data );
	void		 List_set2( struct List *, int index, void* data);
	void		*List_find( struct List *, int (*closure)(void*) );
	void		 List_iterator( struct List *, void (*closure)(void*) );
	void   	   	*List_remove1( struct List *, int index );
	void         List_remove2( struct List *, int index );
	void		 List_removeByPointer( struct List *, void *data );
	void		 List_removeByClosure1( struct List *, int (*closure)(void*) );
	void		 List_removeByClosure2( struct List *, int (*closure)(void*) );
	void		 List_clean( struct List * );
	int			 List_isEmpty( struct List * );
	int			 List_size( struct List * );
	void		*List_destroyer( struct List * );

	static const struct Class _List = {
		"List",
		sizeof(struct List),
		(void*(*)(void*,va_list*))List_constructor, Class_equals, (void*(*)(void*))List_destroyer
	};

	static const void * List = &_List;

	static const struct ListClass ListClass = {
			List_add,
			List_get,
			List_put,
			List_set1,
			List_set2,
			List_find,
			List_iterator,
			List_remove1,
			List_remove2,
			List_removeByPointer,
			List_removeByClosure1,
			List_removeByClosure2,
			List_clean,
			List_isEmpty,
			List_size
	};

#endif /*LIST_H_*/

