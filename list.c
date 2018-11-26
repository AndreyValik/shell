#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

/*Добавляет эелемент списка*/
pid_list pid_add(pid_list list, pid_t pid)
{
	pid_list iter = list;
	pid_list new_node = malloc( sizeof(struct pid_node) );

	new_node->next = NULL;
	new_node->pid = pid;

	if( list == NULL)
		return new_node;

	while( iter->next != NULL )
		iter = iter->next;

	iter->next = new_node;

	return list;
}

/*Удаляет элемент списка*/
pid_list pid_del(pid_list list, pid_t pid)
{
	pid_list iter = list; /*Итератор*/
	pid_list prev = list; /*Предыдущий элемент*/

	while( iter != NULL )
	{
		if( iter->pid != pid )
		{
			prev = iter;
			iter = iter->next;
			continue;
		}

		if( iter == list ) /*Удалить первый элемент списка*/
		{
			list = iter->next;
		}
		else
		{
			prev->next = iter->next;
		}
		free( iter );
		break;
	}

	return list;
}

/*Удаляет весь список, рекурсия*/
void pid_free(pid_list list)
{
	if( list == NULL )
		return;

	pid_free( list->next );
	free( list );
}

str_list str_add(str_list list, const char* str)
{
	str_list iter = list;
	int str_size = strlen( str ) + 1;
	str_list new_node = malloc( sizeof(struct str_node) );

	new_node->next = NULL;
	new_node->str = malloc( str_size );
	memcpy( new_node->str, str, str_size );

	if( list == NULL)
		return new_node;

	while( iter->next != NULL )
		iter = iter->next;

	iter->next = new_node;

	return list;
}

/*Удаляет весь список, рекурсия*/
void str_free(str_list list)
{
	if( list == NULL )
		return;

	str_free( list->next );
	free( list->str );
	free( list );
}

int str_size(str_list list)
{
	int size = 0;

	while( list != NULL )
	{
		size++;
		list = list->next;
	}

	return size;
}

int pid_size(pid_list list)
{
	int size = 0;

	while( list != NULL )
	{
		size++;
		list = list->next;
	}

	return size;
}
