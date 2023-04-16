#include <stdio.h>
#include "terminal.h"
#include "stack.h"
#include "keypad.h"
#include "errors.h"
#include "output.h"

#define CHARARRSIZE 13
#define OUTPUTSIZE 40
#define INITZERO 0

int get_output(char c, Terminal *terminal) {
  printChar(c, terminal);
  return OK;
}

int print_stack(Stack* stack, Terminal *terminal){
	char to_print[CHARARRSIZE] = {INITZERO};
	int length = stack->size;
	for(int i = INITZERO; i < length; i++){
		sprintf(to_print, "%d ", stack->array[i]);
		printTerm(to_print, terminal);
	}
	return OK;
}

int print_top_num(Stack* stack, int n, Terminal *terminal) {
  char top_num_to_print[CHARARRSIZE] = {INITZERO};
	sprintf(top_num_to_print, "%d", n);
	printTerm(top_num_to_print, terminal);
  return OK;
}


int error_output(int is_error, Terminal *terminal) {
	char message_arr[OUTPUTSIZE] = {"/0"};
	
  switch (is_error) {
  case NOTENOUGHPOP: {
    printTerm("Not enough numbers", terminal);
    return OK;
  }
  case ARETHMETICOVERFLOW: {
    printTerm("Overflow", terminal);
    return OK;
  }
  case UNDERFLOW: {
    printTerm("Underflow", terminal);
    return OK;
  }
  case NOTDIVIDEZERO: {
    printTerm("Not dividing by zero",terminal);
    return OK;
  }
  case STACKISFULL: {
    printTerm("Stack is full", terminal);
    return OK;
  }
  case NOTEVENENOUGHFORTOP: {
    printTerm("Not even enough numbers to top", terminal);
    return OK;
  }
  default: {
    return OK;
  }
  }
}