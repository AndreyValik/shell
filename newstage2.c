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
#include "input.h"

int main()
{
	pid_list background_processes = NULL;
	str_list cmd = NULL;

	while( ( cmd = input_cmd() ) != NULL )//если NULL - значит получили EOF
	{
		/*Запуск процесса*/
		if( !is_empty_cmd( cmd ) )
			background_processes = run_process( cmd, background_processes );

		str_free( cmd );

		/*проверим фоновые процессы*/
		background_processes = check_background_processes( background_processes );
	}

	kill_background_processes( background_processes );

	return 0;
}

