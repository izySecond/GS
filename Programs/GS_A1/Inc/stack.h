#ifndef _STACK_H
#define _STACK_H
#define MAX 5

typedef struct struct_stack {
  int array[MAX];
  int size;
} Stack;

int push(Stack *stack, int new_int);
int pop(Stack *stack, int *pop_bag);
int top(Stack *stack, int *top_bag);
int empty(Stack *stack);

#endif