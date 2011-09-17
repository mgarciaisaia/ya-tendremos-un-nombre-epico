#ifndef CONFIGURATION_H
#define CONFIGURATION_H

	#define _XOPEN_SOURCE 500

	#include "oop/Class.h"
	#include "collections/HashMap.h"

	struct Configuration{
		const void * clazz;
		char *path;
		char delimitor;
		struct HashMap *config;
	};

	struct ConfigurationClass{
		char *	(*getString)(struct Configuration *, char *key);
		int 	(*getInt)(struct Configuration *, char *key);
		long 	(*getLong)(struct Configuration *, char *key);
		double 	(*getDouble)(struct Configuration *, char *key);
	};

	struct Configuration*	Configuration_constructor( struct Configuration*, va_list *args );
	char*					Configuration_getString( struct Configuration*, char *key );
	int						Configuration_getInt( struct Configuration*, char *key );
	long					Configuration_getLong( struct Configuration*, char *key );
	double					Configuration_getDouble( struct Configuration*, char *key );
	void*					Configuration_destroyer( struct Configuration* );

	static const struct Class _Configuration = {
		"Configuration",
		sizeof(struct Configuration),
		(void*(*)(void*,va_list*))Configuration_constructor, Class_equals, (void*(*)(void*))Configuration_destroyer
	};

	static const void * Configuration = &_Configuration;

	static const struct ConfigurationClass ConfigurationClass = {
			Configuration_getString,
			Configuration_getInt,
			Configuration_getLong,
			Configuration_getDouble
	};

#endif /* CONFIGURATION_H */
