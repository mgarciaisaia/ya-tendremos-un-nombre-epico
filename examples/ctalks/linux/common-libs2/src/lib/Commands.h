/*
 * Commands.h
 *
 *  Created on: 16/05/2010
 *      Author: facundo
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

	#define _XOPEN_SOURCE 500

	#include <stdarg.h>

	#include "oop/Class.h"
	#include "collections/List.h"
	#include "Array.h"

	struct Command{
		char *name;
		void *context;
		void (*handler)(void*, struct Array *);
	};

	struct Commands{
		const void * clazz;
		struct List *commands;
		void *globalcontext;
		char startArgsToken;
		char endArgsToken;
		char startArgToken;
		char endArgToken;
		char separatorArgsToken;
	};

	struct CommandsClass{
		void (* add1) (struct Commands *, char *name, void (*handler)(void*, struct Array *) );
		void (* add2) (struct Commands *, char *name, void (*handler)(void*, struct Array *), void *context );
		int  (* parser)  (struct Commands *, char *lineCommand );
	};

	struct Commands	*Commands_constructor( struct Commands *self, va_list *args );
	void Commands_add1 (struct Commands *, char *name, void (*handler)(void*, struct Array *) );
	void Commands_add2 (struct Commands *, char *name, void (*handler)(void*, struct Array *), void *context );
	int  Commands_parser (struct Commands *, char *lineCommand );
	void *Commands_destroyer( struct Commands * );

	static const struct Class _Commands = {
		"Commands",
		sizeof(struct Commands),
		(void*(*)(void*,va_list*))Commands_constructor, Class_equals, (void*(*)(void*))Commands_destroyer
	};

	static const void * Commands = &_Commands;

	static const struct CommandsClass CommandsClass = {
			Commands_add1,
			Commands_add2,
			Commands_parser
	};

#endif /* COMMANDS_H_ */
