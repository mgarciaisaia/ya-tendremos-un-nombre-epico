

#ifndef QUEUE_H_
#define QUEUE_H_

	#include <semaphore.h>

	#include "List.h"
	#include "../oop/Class.h"


	typedef enum{
		NONBLOCKING_QUEUE 	= 0X0,
		BLOCKING_QUEUE		= 0X1
	} QueueType;

	struct Queue { extends( List )
		QueueType type;
		sem_t blocking_semaforo;
	};

	struct QueueClass {
		int (*push)( struct Queue *, void *data);
		void *(*pop1) ( struct Queue *);
		void (*pop2) ( struct Queue *);
		void (*iterator)( struct Queue *, void (*closure)(void*) );
		void (*clean)( struct Queue * );
		int (*isEmpty)( struct Queue * );
		int (*size)( struct Queue * );
	};

	struct Queue *Queue_constructor( struct Queue *, va_list *args );
	int			  Queue_push( struct Queue *, void *data );
	void 		 *Queue_pop1( struct Queue * );
	void 		  Queue_pop2( struct Queue * );
	void		  Queue_iterator( struct Queue *, void (*closure)(void*) );
	void		  Queue_clean( struct Queue * );
	int			  Queue_isEmpty( struct Queue * );
	int			  Queue_size( struct Queue * );
	void		 *Queue_destroyer( struct Queue * );

	static const struct Class _Queue = {
		"Queue",
		sizeof(struct Queue),
		(void*(*)(void*,va_list*))Queue_constructor, Class_equals, (void*(*)(void*))Queue_destroyer
	};

	static const void * Queue = &_Queue;

	static const struct QueueClass QueueClass = {
			Queue_push,
			Queue_pop1,
			Queue_pop2,
			Queue_iterator,
			Queue_clean,
			Queue_isEmpty,
			Queue_size
	};

#endif /*QUEUE_H_*/

