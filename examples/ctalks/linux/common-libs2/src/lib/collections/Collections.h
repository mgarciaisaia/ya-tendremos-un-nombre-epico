

#ifndef COLLECTIONS_H_
#define COLLECTIONS_H_

	#include "../oop/Class.h"

	#ifndef DEFAULT_ELEMENTSBLOCK_SIZE
		#define DEFAULT_ELEMENTSBLOCK_SIZE	50
	#endif

	typedef enum{
		UNKWON_TYPE		= 0X1,
		POINTER_TYPE 	= 0X2,
		OBJECT_TYPE 	= 0X3
	} CollectionType;

	struct ElementsBlock{
		const void * clazz;
		char *elements;
		int element_size;
		int elements_free;
		int size;
		struct ElementsBlock *next;
	};

	struct CollectionElement{
	#define CollectionElement_DEF 	\
		void *data;					\
		char  inuse;				\
		void *block;
	CollectionElement_DEF
	};

	struct LinkedElement{ extends( CollectionElement )
	#define LinkedElement_DEF 		\
		struct LinkElement *next;
	LinkedElement_DEF
	};

	struct DoubleLinkedElement{ extends2( CollectionElement, LinkedElement )
		struct DoubleLinkedElement *previous;
	};

	struct HashElement{	extends2( CollectionElement, LinkedElement )
		char *key;
		unsigned int hashcode;
	};

	struct ElementsBlockClass {
		void * (* getFreeElement) (struct ElementsBlock *);
		void * (* get) (struct ElementsBlock *, int index);
		int	   (* size) (struct ElementsBlock *);
		int	   (* totalSize) (struct ElementsBlock *);
	};

	struct ElementsBlock 		*ElementsBlock_constructor( struct ElementsBlock *, va_list *args );
	void						*ElementsBlock_getFreeElement( struct ElementsBlock * );
	void						*ElementsBlock_get( struct ElementsBlock *, int index );
	int							 ElementsBlock_size( struct ElementsBlock * );
	int							 ElementsBlock_totalSize( struct ElementsBlock * );
	void 				  		*ElementsBlock_destroyer( struct ElementsBlock * );

	static const struct Class _ElementsBlock = {
		"ElementsBlock",
		sizeof(struct ElementsBlock),
		(void*(*)(void*,va_list*))ElementsBlock_constructor, Class_equals, (void*(*)(void*))ElementsBlock_destroyer
	};

	static const void * ElementsBlock = &_ElementsBlock;

	static const struct ElementsBlockClass ElementsBlockClass = {
			ElementsBlock_getFreeElement,
			ElementsBlock_get,
			ElementsBlock_size,
			ElementsBlock_totalSize
	};


#endif /* COLLECTIONS_H_ */
