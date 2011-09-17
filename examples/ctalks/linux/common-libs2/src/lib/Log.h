
#ifndef LOG_H_
#define LOG_H_

	#define _XOPEN_SOURCE 500
	#define MAX_LOGMESSAGE_LENGTH 1024

	#include "oop/Class.h"

	typedef enum {
		LOG_ERROR	= 1,
		LOG_WARNING	= 2,
		LOG_INFO	= 3
	}LogLevel;

	typedef enum {
		LOG_OUTPUT_NONE				= 1,
		LOG_OUTPUT_CONSOLE			= 2,
		LOG_OUTPUT_FILE				= 3,
		LOG_OUTPUT_CONSOLEANDFILE	= 4
	}LogOutput;

	struct Log{
		const void * clazz;
		char *path;
		FILE* file;
		int programPID;
		char *programName;
		LogOutput mode;
	};

	struct LogClass{
		int (* info) (struct Log *, char *thread_name, char *format, ... );
		int (* warning) (struct Log *, char *thread_name, char *format, ... );
		int (* error) (struct Log *, char *thread_name, char *format, ... );
	};

	struct Log* Log_constructor( struct Log*, va_list *args );
	int			Log_info(struct Log *, char *thread_name, char *format, ... );
	int			Log_warning(struct Log *, char *thread_name, char *format, ... );
	int			Log_error(struct Log *, char *thread_name, char *format, ... );
	void*		Log_destroyer( struct Log* );

	static const struct Class _Log = {
		"Log",
		sizeof(struct Log),
		(void*(*)(void*,va_list*))Log_constructor, Class_equals, (void*(*)(void*))Log_destroyer
	};

	static const void * Log = &_Log;

	static const struct LogClass LogClass = {
		Log_info,
		Log_warning,
		Log_error,
	};


#endif /*LOG_H_*/
