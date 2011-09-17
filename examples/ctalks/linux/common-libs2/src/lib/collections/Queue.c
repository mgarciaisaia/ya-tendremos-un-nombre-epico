
#include <stdlib.h>
#include <semaphore.h>

#include "../oop/Object.h"
#include "Queue.h"

struct Queue *Queue_constructor( struct Queue *self, va_list *args ){

	self->type = va_arg(* args, QueueType);

	if( self->type == BLOCKING_QUEUE ){
		if( sem_init( &self->blocking_semaforo, 0, 0) == -1 ){
			return NULL;
		}
	}

	return (struct Queue*)List_constructor((struct List*)self, args);
}

int Queue_push( struct Queue *self, void *data ){
	if( self->type == BLOCKING_QUEUE ) sem_post( &self->blocking_semaforo );
	return List_add((struct List*)self, data);
}

void *Queue_pop1( struct Queue *self){
	if( self->type == BLOCKING_QUEUE ) sem_wait( &self->blocking_semaforo );
	return List_remove1((struct List*)self, 0);
}

void Queue_pop2( struct Queue *self){
	if( self->type == BLOCKING_QUEUE ) sem_wait( &self->blocking_semaforo );
	return List_remove2((struct List*)self, 0);
}

void Queue_iterator( struct Queue *self, void (*closure)(void*) ){
	List_iterator((struct List*)self, closure);
}

int Queue_isEmpty( struct Queue *self ){
	return List_isEmpty((struct List*)self);
}

int	Queue_size( struct Queue *self){
	return List_size((struct List*)self);
}

void Queue_clean( struct Queue *self ){
	List_clean((struct List*)self);
}

void *Queue_destroyer( struct Queue *self ){
	sem_destroy( &self->blocking_semaforo );
	return List_destroyer((struct List*)self);
}

