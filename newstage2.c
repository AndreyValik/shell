#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "list.h"
#include "process.h"

int n_arg;
struct info
{
	str_list command;
	char* input;
	char* output;
	int background;
};

void my_cd(char* path)
{
	int f;
	f = chdir( path );
	if( f == -1 )
	{
		printf( "no such file or directory\n" );
	}
}

int main()
{
	pid_list fnlis = NULL;
	pid_list Lfnst;
	int status;
	char c;
	pid_t pid;
	int n = 0;
	str_list L = NULL;
	str_list Lst;
	n_arg = 0;
	int fn_flag = 0;
	char *s = (char*) calloc( 1, sizeof(char) );
	char** arg = NULL;
	while( ( c = getchar() ) != EOF )
	{                     //читаю слова
		if( c != '\n' )
		{
			if( c == '"' )
			{
				if( fn_flag )
				{
					printf( "problem with fn\n" );
					return ( 1 );
				}
				while( ( c = getchar() ) != '"' )
				{
					if( c == EOF )
					{
						printf( "\n" );
						printf( "problem\n" );
						return 1;
					}
					s = realloc( s, strlen( s ) + 1 );
					s[n] = c;
					n++;
					s[n] = 0;
				}
			}
			else
			{
				if( c != ' ' )
				{
					/*if (fn_flag) {
					 printf("problem with fn\n");
					 lfree(L);
					 L=NULL;
					 n_arg=0;
					 return(1);
					 }*/
					s = realloc( s, strlen( s ) + 1 );
					s[n] = c;
					n++;
					s[n] = 0;
					if( c == '&' )
					{
						fn_flag = 1;
					}
				}
				else
				{
					L = str_add(L, s);
					n_arg++;
					n = 0;
				}
			}
		}
		else
		{
			fnlis = check_processes(fnlis);
			if( strlen( s ) != 0 )
			{                     // если не внесли в список последнее слово
				L = str_add( L, s );
				n_arg++;
				n = 0;
			}
			if( L != NULL )
			{                            // переносим все аргументы в массив
				Lst = L;
				arg = realloc( arg, ( n_arg + 1 ) * ( sizeof(char*) ) );
				for( int j = 0; j < n_arg; j++ )
				{
					arg[j] = Lst->str;
					Lst = Lst->next;
				}
				arg[n_arg] = NULL;
				n = 0;
				if( fn_flag )
				{
					if( strcmp( arg[n_arg - 1], "&" ) == 0 )
					{
						arg = realloc( arg, n_arg - 1 );
						arg[n_arg] = NULL;
						pid = fork();
						if( !pid )
						{

							execvp( arg[0], arg );
							fprintf( stderr, "Error executing %s: %s\n", arg[0], strerror( errno ) );
							exit( 1 );
						}

						fnlis = pid_add( fnlis, pid );
						printf( "running the background process with PID=%d\n", pid );
						fn_flag = 0;
						str_free( L );
						L = NULL;
						n_arg = 0;

					}
					else
					{
						printf( "problem with fn\n" );
						str_free( L );
						L = NULL;
						n_arg = 0;
						fn_flag = 0;
					}
				}
				else if( strcmp( arg[0], "cd" ) == 0 )
				{
					if( n_arg > 2 )
					{
						printf( "problem with cd\n" );
						str_free( L );
						L = NULL;
						n_arg = 0;
						//return(1);
					}
					else
					{
						my_cd( arg[1] );
						str_free( L );
						L = NULL;
						n_arg = 0;
					}
				}
				else
				{
					if( !fork() )
					{
						execvp( arg[0], arg );
						exit( 1 );
					}
					else
					{
						wait( NULL );
						str_free( L );
						L = NULL;
						n_arg = 0;
					}
				}
			}
		}
	}
	free( s );
	int fl = 0;

	if( L == NULL )
		fl = 1;
	if( !fl )
		printf( "\n" );
}

