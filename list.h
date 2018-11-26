#ifndef _LIST_H_
#define _LIST_H_

#include <sys/types.h>

/*Список строк*/
typedef struct str_node *str_list;
struct str_node {
	char* str;
	str_list next;
};

/*Список pid-ов*/
typedef struct pid_node *pid_list;
struct pid_node {
	pid_t pid;
	pid_list next;
};



/*Добавляет эелемент списка*/
pid_list pid_add(pid_list list, pid_t pid);

/*Удаляет элемент списка*/
pid_list pid_del(pid_list list, pid_t pid);

/*Удаляет весь список, рекурсия*/
void pid_free(pid_list list);

/*Добавляет эелемент списка*/
str_list str_add(str_list list, const char* str);

/*Удаляет весь список, рекурсия*/
void str_free(str_list list);

#endif /*_LIST_H_*/
