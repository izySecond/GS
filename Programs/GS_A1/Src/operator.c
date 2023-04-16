#include "operator.h"
#include "errors.h"
#include "output.h"
#include "stack.h"
#define INITZERO 0
#define COMPAREZERO 0

int determine_oparator(Stack *stack, char c, Terminal *terminal) {

  int num1 = INITZERO;
  int num2 = INITZERO;
  int top_number = INITZERO;
  int error_code = INITZERO;
  error_code = top(stack, &top_number);

  if (COMPAREZERO > error_code) {
    return error_code;
  }
	
  switch (c) {
		case '+': {
			error_code = pop(stack, &num1);
			error_code = pop(stack, &num2);
			int new_int_add = num1 + num2;
			push(stack, new_int_add);
			break;
  }
		case '-': {
			error_code = pop(stack, &num1);
			error_code = pop(stack, &num2);
			int new_int_sub = num1 - num2;
			push(stack, new_int_sub);
			break;
  }
		case '/':{
			error_code = pop(stack, &num1);
			error_code = pop(stack, &num2);
			if (COMPAREZERO == num1) {
				return NOTDIVIDEZERO;
    }
			int new_int_div = num2 / num1;
			push(stack, new_int_div);
			break;
	}
		case '*': {
			error_code = pop(stack, &num1);
			error_code = pop(stack, &num2);
			int new_int_mul = num1 * num2;
			if (COMPAREZERO != num1 && new_int_mul / num1 != num2) {
				return ARETHMETICOVERFLOW;
    }
			push(stack, new_int_mul);
			break;
  }
		case ' ':{
			break;
	}
		case '#': {
			break;
	}
		case 'e':{
			break;
	}
		case 'C':{
			empty(stack);
			break;
	}

		case 'p': {
			print_top_num(stack, top_number, terminal);
			break;
  }

		case 'P':{
			print_stack(stack, terminal);
			break;}

		case 'd':{
			push(stack, top_number);
			break;}

		case 'r':{
;
			push(stack, num1);
			push(stack, num2);
			break;}

		default:{
			break;}
  }
		return OK;
}

int get_nums(Stack stack, int* &num1_ad, int* &num2_ad){
	int error_code = OK;
	error_code = pop(stack, &num1_ad);
	error_code = pop(stack, &num2_ad);
	return error_code;
}