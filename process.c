#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "process.h"

int check_cd(str_list strings)
{
	char* dir = "~/"; //Домашний каталог по умолчанию, если cd без аргументов

	if( strings == NULL )
		return 0;

	if( strcmp( strings->str, "cd") != 0 )
		return 0;

	if( strings->next != NULL )
	{
		dir = strings->next->str;
	}

	if( chdir( dir ) != 0 )
	{
		fprintf( stderr, "cd: \"%s\": %s\n", strings->next->str, strerror( errno ) );
	}

	return 1;
}

pid_list check_background_processes(pid_list list)
{
	pid_t pid;
	int status;

	while( ( pid = waitpid( -1, &status, WNOHANG ) ) > 0 )
	{
		if( WIFEXITED( status ) )
		{
			list = pid_del( list, pid );
			printf( "process with PID=%d finished with status %d\n", pid, WEXITSTATUS( status ) );
		}
		else if( WIFSIGNALED( status ) )
		{
			list = pid_del( list, pid );
			printf( "process with PID=%d terminated by signal %d\n", pid, WTERMSIG( status ) );
		}
	}

	return list;
}

pid_list run_process(str_list strings, pid_list pids)
{
	char** args;
	int nargs = str_size( strings );
	int background = 0;
	pid_t pid;

	if( check_cd(strings) )
		return pids;

	args = malloc( ( nargs + 1 ) * sizeof( nargs ) );
	nargs = 0;
	while( strings != NULL )
	{
		args[nargs] = strings->str;
		nargs++;
		strings = strings->next;
	}

	if( args[nargs - 1][0] == '&' )
	{
		nargs--;
		background = 1;
	}

	args[nargs] = NULL;

	pid = fork();
	if( !pid )
	{

		execvp( args[0], args );
		fprintf( stderr, "Error executing %s: %s\n", args[0], strerror( errno ) );
		exit( 1 );
	}

	if( background )
	{
		pids = pid_add( pids, pid );
		printf( "running the background process with PID=%d\n", pid );
	}
	else
	{
		int status;
		pid_t ret;

		while( ( ret = waitpid( pid, &status, 0 ) ) >= 0 )
		{
			if( ret != pid )
				continue;

			if( WIFEXITED( status ) )
			{
				break;
			}
			else if( WIFSIGNALED( status ) )
			{
				printf( "process with terminated by signal %d\n", WTERMSIG( status ) );
				break;
			}
		}
	}

	free( args );

	return pids;
}
