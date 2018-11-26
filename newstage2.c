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
	pid_list fnlis = NULL;
	str_list L = NULL;

	while( ( L = input_cmd() ) != NULL )
	{
		/*Запуск процесса*/
		fnlis = run_process( L, fnlis );

		str_free( L );

		/*проверим фоновые процессы*/
		fnlis = check_background_processes( fnlis );
	}

	while( (fnlis = check_background_processes( fnlis )) != NULL );

	return 0;
}

