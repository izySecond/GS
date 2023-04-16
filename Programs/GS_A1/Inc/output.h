#ifndef _OUTPUT_H
#define _OUTPUT_H
#include "terminal.h"
#include "stack.h"


int get_output(char c, Terminal* terminal);
int print_stack(Stack* stack, Terminal *terminal);
int print_top_num(Stack* stack, int n, Terminal *terminal);
int error_output(int is_error, Terminal *terminal);

#endif