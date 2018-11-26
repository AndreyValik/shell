#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

/*Проверить что строка заканчивается на спец-разделитель*/
char* check_cmd_separator(char* str)
{
	char* separators[] =
	{ "\n", "&\n" };
	int len;

	if( !str )
		return NULL;

	len = strlen( str );

	for( int i = 0; i < sizeof( separators ) / sizeof( separators[0] ); i++ )
	{
		int sep_len = strlen( separators[i] );
		if( len < sep_len )
		{
			continue;
		}
		if( strcmp( str + len - sep_len, separators[i] ) == 0 )
		{
			return str + len - sep_len;
		}
	}

	return NULL;
}

//прочитать строку
char* input_line()
{
	char buf[128];
	char* str = malloc( 1 );

	str[0] = 0;

	while( fgets( buf, sizeof( buf ), stdin ) )
	{
		int buf_len = strlen( buf );
		int str_len = strlen( str );

		str = realloc( str, str_len + buf_len + 1 );
		memcpy( str + str_len, buf, buf_len + 1 );
		if( buf[buf_len - 1] == '\n' )
		{
			str[str_len + buf_len - 1] = 0;
			return str;
		}
	}

	return NULL;
}

char* input_arg()
{
	int c;
	int in_quotes = 0;
	int ampersand_position = -1;
	int input = 1;
	int position = 0;
	char* str = malloc( 1 );

	str[0] = 0;

	while( input )
	{
		c = getchar();
		if( c == EOF ) //если конец файла, то выход
		{
			free( str );
			return NULL;
		}

		if( in_quotes ) //если ввод аргумента в кавычках
		{
			if( c == '"' )
			{
				in_quotes = 0;
				continue;
			}
		}
		else
		{
			//проверим вводимый символ на спец-символы
			switch( c )
			{
				case '"': //начало аргумента в кавычках
				{
					in_quotes = 1;
					continue;
				}

				case '&': //запомним позицию амперсанда, а вообще он должен быть разделителем таким же как и пробел
				{
					ampersand_position = position;
					break;
				}

				case ' ':
				case '\t':
				{
					if( position > 0 ) //начальные пробелы и табуляции пропускаем, иначе завершение ввода аргумента
					{
						input = 0;
						c = 0;
					}
					break;
				}
			}
		}

		if( ( ampersand_position == 0 )
				|| ( ( ampersand_position > 0 ) && ( ampersand_position == ( position - 1 ) ) && ( c != '\n' ) ) )
		{
			fprintf( stderr, "\nWrong position of '&'!\n" );
			str = realloc( str, 1 );
			str[0] = 0;
			ampersand_position = -1;
			position = 0;
			input = 1;
			continue;
		}

		if( c )
		{
			str[position++] = c;
			str = realloc( str, position + 1 );
			str[position] = 0;
		}
	}

	return str;
}

int check_newline(const char* str)
{
	int len = strlen( str );

	if( ( len > 0 ) && ( str[len - 1] == '\n' ) )
		return 1;

	return 0;
}

int check_ampersand(const char* str)
{
	int len = strlen( str );

	if( ( len > 1 ) && ( str[len - 2] == '&' ) )
		return 1;

	return 0;
}

str_list input_cmd()
{
	char* arg;
	int newline = 0;
	str_list list = NULL;

	while( ( arg = input_arg() ) )
	{
		int len = strlen( arg );
		int newline = check_newline( arg );
		int background = check_ampersand( arg );

		if( !len ) //пустой аргумент, пропускаем
			continue;

		list = str_add( list, arg );

		if( newline ) //если был перевод строки - конец ввода команды
		{
			if( background )
				list = str_add( list, "&" ); //добавим отдельным параметром

			list = str_add( list, "\n" ); //добавим  отдельным параметром

			return list;
		}
	}

//если NULL, то значит получили EOF
	return NULL;
}

int is_empty_cmd(str_list list)
{
	if( list && ( list->str[0] == '\n' ) )
		return 1;

	return 0;
}
