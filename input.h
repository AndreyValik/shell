#ifndef _INPUT_H_
#define _INPUT_H_

#include "list.h"

//ввод команды
str_list input_cmd();

//проверка что ввели просто перевод строки
int is_empty_cmd(str_list list);

#endif /*_INPUT_H_*/
