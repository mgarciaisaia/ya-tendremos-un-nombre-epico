#ifndef NSTRING_H
#define NSTRING_H

	#define _XOPEN_SOURCE 500

	#include "oop/Class.h"

	struct NStringClass{
		int (*startWith)(char *str, char *prefix);
		int (*asInt)(char *str);
		double (*asDouble)(char *str);
		int (*length)(char *str);
		char**(*split)(char *str, char delimitor);
		char* (*trim)(char *str);
		char* (*trimInBuffer)(char *str, char *buffer);
		char* (*clone)(char *str);
		int	  (*indexOf1)( char *str1, char *str2);
		int	  (*indexOf2)( char *str1, int offset, char *str2);
		int (*equals)(char *str1, char *str2);
	};

	int		NString_startWith( char *str, char *prefix );
	int		NString_asInt( char *str );
	double	NString_asDouble( char *str );
	int		NString_length( char *str );
	char**	NString_split( char *str, char delimitor );
	char*	NString_trim( char *str );
	char* 	NString_trimInBuffer( char *str, char *buffer );
	char*	NString_clone( char *str );
	int		NString_indexOf1( char *str1, char *str2);
	int		NString_indexOf2( char *str1, int offset, char *str2);
	int		NString_equals( char *str1, char *str2 );


	static const struct NStringClass NStringClass = {
			NString_startWith,
			NString_asInt,
			NString_asDouble,
			NString_length,
			NString_split,
			NString_trim,
			NString_trimInBuffer,
			NString_clone,
			NString_indexOf1,
			NString_indexOf2,
			NString_equals
	};

#endif /* NSTRING_H */

