
/*
 *	@FILE: 	collections/list.h
 *	@AUTOR: Facundo Viale
 *	@VERSION:	1.6	(2/09/2009)
 *					- Mayor funcionalidad con Closures
 *				1.5	(27/08/2009)
 *					- Full Revision
 *				1.0	(26/08/2008)
 *					- Base Version
 */

#ifndef OLD_LIST_H_
#define OLD_LIST_H_

	#define _XOPEN_SOURCE 500

	#include <semaphore.h>
	#include "collections.h"

	typedef struct{
		t_link_element *head;
		int elements_count;
		sem_t semaforo;
	}t_list;

	t_list     *collection_list_create();
	int     	collection_list_add( t_list *list, void *data );
	void       *collection_list_get( t_list *list, int num );
	void		collection_list_put( t_list *list, int num, void *data );
	void	   *collection_list_switch( t_list* list, int num, void* data );
	void 		collection_list_set( t_list* list, int num, void* data, void (*data_destroyer)(void*));
	void	   *collection_list_find( t_list *list, int (*closure)(void*) );
	void		collection_list_iterator( t_list *list, void (*closure)(void*) );
	void   	   *collection_list_remove( t_list *list, int num );
	void        collection_list_removeAndDestroy( t_list *list, int num, void (*data_destroyer)(void*) );
	void    	collection_list_removeByPointer( t_list *list, void *data, void (*data_destroyer)(void*) );
	void    	collection_list_removeByClosure( t_list *list, int (*closure)(void*), void (*data_destroyer)(void*) );
	void	   *collection_list_removeByClosure2( t_list *list, int (*closure)(void*) );
	int     	collection_list_size( t_list *list );
	int     	collection_list_isEmpty( t_list *list );
	void 		collection_list_clean( t_list *list, void (*data_destroyer)(void*) );
	void 		collection_list_destroy( t_list *list, void (*data_destroyer)(void*) );

#endif /*OLD_LIST_H_*/

