#include "stack.h"
#include "errors.h"
#include <ctype.h>
#include <stdio.h>

int push(Stack *stack, int new_int) {
  if (stack->size >= MAX) {
    return STACKISFULL;
  }
  stack->array[stack->size] = new_int;
  stack->size++;
  return OK;
}

int pop(Stack *stack, int *pop_bag) {
  if (stack->size <= 0) {
    return NOTENOUGHPOP;
  }
  int pop_num = stack->array[stack->size - 1];
  *pop_bag = pop_num;
  stack->size--;
  return OK;
}

int top(Stack *stack, int *top_bag) {
  if (stack->size == 0) {
    return NOTEVENENOUGHFORTOP;
  }
  int top_num = stack->array[stack->size - 1];
  *top_bag = top_num;
  return OK;
}

int empty(Stack *stack) {
  stack->size = 0;
  return OK;
}
