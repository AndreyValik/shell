#include <stdio.h>
#include <sys/wait.h>

#include "process.h"

pid_list check_processes(pid_list list)
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
