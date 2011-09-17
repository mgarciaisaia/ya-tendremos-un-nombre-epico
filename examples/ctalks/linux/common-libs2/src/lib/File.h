
#ifndef FILE_H_
#define FILE_H_

	#define FILE_NOT_FOUND -1
	#define _XOPEN_SOURCE 500

	#include <stdio.h>

	#include "oop/Class.h"

	struct File{
		const void * clazz;
		FILE *desc;
		char *path;
		char *text;
		int numberOfLines;
		int maxLineLength;
	};

	struct FileClass{
		int (*exist)(struct File *);
		int (*isDirectory)(char *path);
		int (*create)(struct File *);
		int (*delete)(struct File *);
		int (*size)(struct File *);
		char* (*read)(struct File *);
		void (*iterateLines)(struct File *, void (*closure)(char* line));
		int (*write)(struct File *);
	};

	struct File* File_constructor( struct File*, va_list *args );
	int			 File_exist( struct File* );
	int			 File_isDirectory( char *path );
	int			 File_create( struct File* );
	int			 File_delete( struct File* );
	int			 File_size( struct File* );
	char*		 File_read( struct File* );
	void		 File_iterateLines( struct File*, void (*closure)(char* line) );
	int			 File_write( struct File* );
	void*		 File_destroyer( struct File* );

	static const struct Class _File = {
		"File",
		sizeof(struct File),
		(void*(*)(void*,va_list*))File_constructor, Class_equals, (void*(*)(void*))File_destroyer
	};

	static const void * File = &_File;

	static const struct FileClass FileClass = {
		File_exist,
		File_isDirectory,
		File_create,
		File_delete,
		File_size,
		File_read,
		File_iterateLines,
		File_write
	};


#endif /*FILE_H_*/
