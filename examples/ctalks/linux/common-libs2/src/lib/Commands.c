/*
 * Commands.c
 *
 *  Created on: 16/05/2010
 *      Author: facundo
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "oop/Object.h"

#include "NString.h"

#include "Commands.h"

static struct Array *Commands_split( char *str, const char sep, char startTokenDelimitor, char endTokenDelimitor);

struct Commands	*Commands_constructor( struct Commands *self, va_list *args ){

	self->commands = new( List, POINTER_TYPE, free);

	self->globalcontext =  va_arg(* args, void*);

	self->startArgsToken = va_arg(* args, int);
	self->endArgsToken = va_arg(* args, int);

	self->separatorArgsToken = va_arg(* args, int);

	self->startArgToken = va_arg(* args, int);
	self->endArgToken = va_arg(* args, int);

	return self;
}

void Commands_add1 (struct Commands *self, char *name, void (*handler)(void*, struct Array *) ){
	Commands_add2(self, name, handler, NULL);
}

void Commands_add2 (struct Commands *self, char *name, void (*handler)(void*, struct Array *), void *context ){
	struct Command *command = malloc( sizeof(struct Command) );
	command->name = name;
	command->context = context;
	command->handler = handler;

	ListClass.add(self->commands, command);
}

static struct Array *Commands_split( char *str, const char sep, char startTokenDelimitor, char endTokenDelimitor){
	struct Array *str_split = NULL;
	int cont, arg_pibot, arg_size, arg_end;
	int delimitorBlock = 0;

	for(arg_size=1, cont=0;  str[cont] != '\0'; cont++ ){
		if( str[cont] == startTokenDelimitor && startTokenDelimitor != '\0' && delimitorBlock == 0 ){
			delimitorBlock = 1;
		}else if( str[cont] == endTokenDelimitor && endTokenDelimitor != '\0' && delimitorBlock == 1 ){
			delimitorBlock = 0;
		}else if( str[cont] == sep && delimitorBlock == 0 ){
			arg_size++;
		}
	}

	str_split = new( Array, POINTER_TYPE, free, arg_size);

	for(arg_pibot=0, cont=0; cont<ArrayClass.size(str_split); cont++ ){
		char *substr;

		delimitorBlock = 0;
		for(arg_size=-1, arg_end=0; ; arg_end++ ){
			if( str[arg_end+arg_pibot] == startTokenDelimitor && startTokenDelimitor != '\0' && delimitorBlock == 0 ){
				delimitorBlock = 1;
				arg_pibot++;
			}else if( str[arg_end+arg_pibot] == endTokenDelimitor && endTokenDelimitor != '\0' && delimitorBlock == 1 ){
				delimitorBlock = 0;
				arg_size = arg_end;
			}else if( (str[arg_end+arg_pibot] == sep && delimitorBlock == 0) || str[arg_end+arg_pibot] == '\0' ){
				if( arg_size == -1 ) arg_size = arg_end;
				break;
			}
		}

		substr = malloc( arg_size + 1 );
		strncpy(substr, &str[arg_pibot], arg_size);
		substr[arg_size] = '\0';

		ArrayClass.set1(str_split, cont, substr);

		arg_pibot = arg_pibot + arg_end + 1;
	}

	return str_split;
}

int  Commands_parser (struct Commands *self, char *lineCommand ){
	struct Command *command = NULL;
	struct Array *args = NULL;

	int lineCommandlenght = strlen(lineCommand);

	int find_command(struct Command *cmd){ return NStringClass.startWith(lineCommand, cmd->name); }


	if( (command = ListClass.find(self->commands, (void*)find_command)) != NULL ){

		if( lineCommand[ strlen(command->name) ] == self->startArgsToken  && (self->endArgsToken == '\0' ||  lineCommand[ lineCommandlenght - 1 ] == self->endArgsToken ) ){
			char subargsbuffer[lineCommandlenght];
			int tokendiff = 2;

			if( self->endArgsToken == '\0') tokendiff = 1;

			memcpy(subargsbuffer, &lineCommand[ strlen(command->name) + 1 ], lineCommandlenght - strlen(command->name) - tokendiff );

			subargsbuffer[lineCommandlenght - strlen(command->name) - tokendiff] = '\0';


			if( strlen(subargsbuffer) > 0 ){
				args = Commands_split(subargsbuffer, self->separatorArgsToken, self->startArgToken, self->endArgToken );
			}
		}

		if(command->context != NULL){
			command->handler(command->context, args);
		}else{
			command->handler(self->globalcontext, args);
		}

		return 1;
	}

	return 0;
}

void *Commands_destroyer( struct Commands *self ){
	delete( self->commands );
	return self;
}
