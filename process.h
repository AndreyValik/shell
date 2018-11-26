#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "list.h"

/*Проверка фоновых процессов*/
pid_list check_background_processes(pid_list list);

/*Запустить процесс, если фоновый, то добавляет в список фоновых процессов*/
pid_list run_process(str_list strings, pid_list pids);


#endif /*_PROCESS_H_*/
