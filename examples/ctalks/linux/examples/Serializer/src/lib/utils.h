/*
 * utils.h
 *
 *  Created on: 02/08/2009
 *      Author: Matias Dumrauf
 */


#ifndef OLD_UTILS_H_
#define OLD_UTILS_H_

	#define _XOPEN_SOURCE 500

	#include "collections/list.h"


	typedef enum{
		HTML_FILE, JPG_FILE, BMP_FILE, PNG_FILE, GIF_FILE, ANY_FILE
	} e_file_type;

	#define DEFAULT_SPRINTF_BUFFER 500


	char **utils_convertList_intoDoublePointerList(t_list *list);

	int    utils_numdigitsOfInt(int x);
	int    utils_numdigitsOfLong(long x);
	int    utils_numdigitsOfFloat(float x);

	char  *utils_convertIntToString(int x);
	char  *utils_convertIntToStringBuffer(int x, char buffer[]);
	char  *utils_convertLongToString(long x);
	char  *utils_convertFloatToString(float f);

	char  *utils_string_replace(const char *s1, const char *s2, const char *s3);
	int    utils_string_compare(char *s1, char *s2);
	char  *utils_string_clone(char *s);
	char  *utils_string_buildFromTemplate(const char *template, ...);

	int    utils_decodeFileExtension(char *extension);

	void   utils_url_decodeHost(char *url,/*out*/char **ip,/*out*/int *port);
	char  *utils_url_getFileName(char *url);

	int	   utils_isEven(int num);
	int	   utils_isOdd(int num);

	int	   utils_isMemSettedWith(char *mem, int memsize, int byte);

#endif /*OLD_UTILS_H_*/
