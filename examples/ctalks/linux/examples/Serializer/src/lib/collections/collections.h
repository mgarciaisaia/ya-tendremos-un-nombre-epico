
/*
 *	@FILE: 	collections/collections.h
 *	@AUTOR: Facundo Viale
 *	@VERSION:	1.5	(27/08/2009)
 *					- Full Revision
 *				1.0	(01/09/2008)
 *					- Base Version
 */


#ifndef OLD_COLLECTIONS_H_
#define OLD_COLLECTIONS_H_

	#define _XOPEN_SOURCE 500

	#include <semaphore.h>

	struct link_element{
		void *data;
		struct link_element *next;
	};
	typedef struct link_element t_link_element;

	struct dlink_element{
		struct dlink_element *previous;
		void *data;
		struct dlink_element *next;
	};
	typedef struct dlink_element t_dlink_element;

#endif /*OLD_COLLECTIONS_H_*/
