#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <pthread.h>

#include "Log.h"

int Log_write ( struct Log *, char *thread_name , LogLevel level , const char* format, va_list args_list );
char *Log_LevelAsString( LogLevel level );

struct Log* Log_constructor( struct Log *self, va_list *args ){
	char* path = va_arg(* args, char*);
	char* programName = va_arg(* args, char*);

	self->path = malloc( strlen(path) + 1 );
	strcpy(self->path, path);

	self->programName = malloc( strlen(programName) + 1 );
	strcpy(self->programName, programName);

	self->mode = va_arg(* args, LogOutput);

	self->programPID = getpid();

	if( self->mode == LOG_OUTPUT_FILE || self->mode == LOG_OUTPUT_CONSOLEANDFILE ){
		self->file = fopen( self->path , "a");
	}

	return self;
}

int	Log_info(struct Log *self, char *thread_name, char *format, ... ){
	va_list args_list;
	va_start(args_list, format);
	return Log_write(self, thread_name, LOG_INFO, format, args_list);
}

int	Log_warning(struct Log *self, char *thread_name, char *format, ... ){
	va_list args_list;
	va_start(args_list, format);
	return Log_write(self, thread_name, LOG_WARNING, format, args_list);
}

int	Log_error(struct Log *self, char *thread_name, char *format, ... ){
	va_list args_list;
	va_start(args_list, format);
	return Log_write(self, thread_name, LOG_ERROR, format, args_list);
}


int Log_write( struct Log *self, char *thread_name , LogLevel level , const char* format, va_list args_list ){
	time_t log_time = time(NULL);
	struct tm *log_tm = localtime( &log_time );
	struct timeb tmili;
	char str_time[128] ;
	unsigned int thread_id = pthread_self();
	char *str_type = Log_LevelAsString(level);
	char logbuff[MAX_LOGMESSAGE_LENGTH+100];
	char msgbuff[MAX_LOGMESSAGE_LENGTH];

	if( self->mode == LOG_OUTPUT_NONE )	return 1;

	vsprintf(msgbuff, format, args_list);

	strftime( str_time , 127 , "%H:%M:%S" , log_tm ) ;

	if (ftime (&tmili)){
		perror("[[CRITICAL]] :: No se han podido obtener los milisendos\n");
		return 0;
	}

	sprintf( logbuff, "%s:%hu %s/%d %s/%u %s: %s\n",str_time, tmili.millitm, self->programName , self->programPID , thread_name , thread_id , str_type , msgbuff );

	if( self->mode == LOG_OUTPUT_FILE || self->mode == LOG_OUTPUT_CONSOLEANDFILE ){
		fprintf( self->file , "%s", logbuff );
		fflush( self->file ) ;
	}
	if( self->mode == LOG_OUTPUT_CONSOLE || self->mode == LOG_OUTPUT_CONSOLEANDFILE ){
		printf("%s", logbuff);
	}

	return 1;
}

char *Log_LevelAsString( LogLevel level ){
	if( level == LOG_ERROR)		return "ERROR";
	if( level == LOG_WARNING)	return "WARNING";
	if( level == LOG_INFO)		return "INFO";
	return "UNKNOW_LOG_LEVEL";
}

void* Log_destroyer( struct Log *self ){
	if( self->mode == LOG_OUTPUT_FILE || self->mode == LOG_OUTPUT_CONSOLEANDFILE ) fclose(self->file);
	free( self->path );
	free( self->programName );
	return self;
}
