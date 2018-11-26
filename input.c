#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "input.h"

/*Читаем строку до конца*/
char* input_line()
{
	char buf[5];
	char* str = malloc( 1 );
	str[0] = 0;

	while( fgets( buf, sizeof( buf ), stdin ) )
	{
		int len = strlen( buf );
		if( len == 0 )
			continue;

		str = realloc( str, strlen( str ) + len + 1 );
		strcat( str, buf );
		if( buf[len - 1] == '\n' )
			return str;
	}

	free( str );
	return NULL;
}

str_list input_cmd()
{
	char* line = input_line();
	str_list list = NULL;

	if( line == NULL )
		return NULL;

	return list;
}
