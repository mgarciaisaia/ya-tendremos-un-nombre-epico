#ifndef STRING_H
#define STRING_H

	#define _XOPEN_SOURCE 500

	#include "oop/Class.h"
	#include "Array.h"

	struct String{
		const void * clazz;
		char* text;
		int length;
	};

	struct StringClass{
		int (*startWith)(struct String *, char *prefix);
		int (*asInt)(struct String *);
		double (*asDouble)(struct String *);
		struct Array * (*split)(struct String *, char delimitor);
		struct String * (*trim)(struct String *);
		struct String * (*clone)(struct String *);
		int (*equals)(struct String *, struct String *);
	};

	struct String*	String_constructor( struct String*, va_list *args );
	int				String_startWith( struct String*, char *prefix );
	int				String_asInt( struct String* );
	double			String_asDouble( struct String* );
	struct Array *  String_split(struct String *, char delimitor);
	struct String*	String_trim( struct String* );
	struct String*	String_clone( struct String* );
	int				String_equals( struct String*, struct String* );
	void*			String_destroyer( struct String* );

	static const struct Class _String = {
		"String",
		sizeof(struct String),
		(void*(*)(void*,va_list*))String_constructor, Class_equals, (void*(*)(void*))String_destroyer
	};

	static const void * String = &_String;

	static const struct StringClass StringClass = {
			String_startWith,
			String_asInt,
			String_asDouble,
			String_split,
			String_trim,
			String_clone,
			String_equals
	};

#endif /* STRING_H */
