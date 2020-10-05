#include "stack.h"

#define ASSERT_OK(stack)							\
    if(stackOk_simple(stack) != NO_ERROR)			\
    {												\
        stackDump_simple(stack);					\
        assert(0);									\
    }												\
    

int isZero(elem_t value)
{
	return (fabs(value) < EPS);
}

int checkSum(Stack* stack)
{
	const int heshy = 15487457 * 23;

	int check_sum = (int)stack;

	//add canarry in function
	//int canarry = (int)stack->buffer + (int)(stack->buffer + stack->capacity);

	check_sum += (int)stack->capacity;
	check_sum += (int)stack->size;

	
    for (size_t i = 0; i < stack->size * sizeof(elem_t); ++i)
    {
		check_sum += ((int)((char*)stack->buffer + i)) % heshy;
    }

	for (size_t i = 0; i < stack->size * sizeof(elem_t); ++i)
	{
		check_sum += ((int)((char*)stack->buffer + i) * (i* (i + 1))) % heshy;
	}

	for (size_t i = 0; i < stack->size; ++i)
	{
		check_sum += (int)stack->buffer[i];
	}

	for (size_t i = 0; i < stack->size; ++i)
	{
		check_sum += (int)stack->buffer[i] * (i + 1);

		if (i % 100 == 0)
		{
			check_sum %= heshy;
		}
	}

	check_sum += (int)stack->buffer;

    return check_sum;
}

void fillNolls(elem_t* start, elem_t* end)
{
    for (int i = 0; i < end - start + 1; ++i)
    {
		start[i] = NOLL;
    }
}

void constructStack_simple(Stack* stack, const size_t start_size)
{
	stack->buffer = (elem_t*)calloc(start_size, sizeof(elem_t));
	stack->capacity = start_size;
	stack->size = 0;

	fillNolls(stack->buffer + stack->size, stack->buffer + stack->capacity);

	stack->check_sum = checkSum(stack);

	ASSERT_OK(stack);

}

void constructStack_smart(Stack_s* stack, const size_t start_size)
{
	stack->capacity = start_size;
	stack->size = 0;
}

Stack* newStack_simple(const size_t start_size)
{
	Stack* new_stack = (Stack*)calloc(1, sizeof(Stack));

	constructStack_simple(new_stack, start_size);

	ASSERT_OK(new_stack);

	return new_stack;
}

Stack_s* newStack_smart(const size_t start_size)
{
	Stack_s* stack = (Stack_s*)calloc(1, sizeof(Stack_s) + start_size * sizeof(elem_t));

	constructStack_smart(stack, start_size);

	return stack;

}

void changeCapacity(Stack* stack, const double factor, const OPERATION operation)
{
	//printf("CHANGE CAPACITY: %d\n", stack->capacity);

	if (operation == PUSH)
	{
		stack->capacity = factor * stack->size;
	}
	else if (operation == POP)
	{
		stack->capacity = (size_t)(stack->capacity / factor) + 1;
	}
	else
	{
		return;
	}

	//printf("CHANGE CAPACITY: %d\n", stack->capacity);


	stack->buffer = (elem_t*)realloc(stack->buffer, stack->capacity * sizeof(elem_t));

}

void checkSizeCapacity(Stack* stack, const OPERATION operation)
{
	if (stack->size + 1 > stack->capacity && operation == PUSH)
	{
		changeCapacity(stack, 2, operation);
	}
	else if (stack->size < stack->capacity / 1.4 && operation == POP)
	{
		changeCapacity(stack, 1.4, operation);

		fillNolls(stack->buffer + stack->size, stack->buffer + stack->capacity);
	}
	
}

void pushStack_simple(Stack* stack, const elem_t value)
{
	ASSERT_OK(stack);
	
	checkSizeCapacity(stack, PUSH);

	stack->buffer[stack->size] = value;
	++stack->size;

	stack->check_sum = checkSum(stack);
	fillNolls(stack->buffer + stack->size, stack->buffer + stack->capacity);

	ASSERT_OK(stack);

}

elem_t topStack_simple(Stack* stack)
{
	ASSERT_OK(stack);

	return stack->buffer[stack->size - 1];

}

void popStack_simple(Stack* stack)
{
	ASSERT_OK(stack);

	checkSizeCapacity(stack, POP);

	stack->buffer[stack->size - 1] = NOLL;
	--(stack->size);

	stack->check_sum = checkSum(stack);


	ASSERT_OK(stack);
}

void deleteStack_simple(Stack* stack)
{
	ASSERT_OK(stack);

	free(stack->buffer);
	free(stack);
}

void deleteStack_smart(Stack_s* stack)
{
	free(stack);
}

ERROR_MESSAGE stackOk_simple(Stack* stack)
{
	static size_t count = 0;
	count++;

	/*
	printf("FUNCTION CALL # %d\n", count);
	printf("===\n");

	
	printf("CHECK SUM NATIVE: %d\n", stack->check_sum);
	printf("CHECK SUM AFTER: %d\n", test_check_sum);
	printf("===\n");
	*/
	

	if (stack == NULL)
	{
		return NULL_STACK_ERROR;
	}
	else if (stack->buffer == NULL)
	{
		return NULL_BUFFER_ERROR;
	}
	
	else if (stack->size == 0 && stack->capacity == 0)
	{
		return ZERO_SIZE_ERROR;
	}
	
	else if (stack->capacity == 0)
	{
		return ZERO_CAPACITY_ERROR;
	}
	else if (stack->capacity < stack->size)
	{
		return BIG_SIZE_ERROR;
	}

	const int test_check_sum = checkSum(stack);

	if (stack->check_sum != test_check_sum)
	{
		return  CHANGE_SOME_DATA_ERROR;
	}

	else
	{
	    for (size_t i = 0; i < stack->size; ++i)
	    {
	        if (isZero(stack->buffer[i] - NOLL))
	        {
				return NOLL_ELEMENT_ERROR;
	        }
	    }
	}

	return NO_ERROR;
	
}

void stackDump_simple(Stack* stack)
{
	const ERROR_MESSAGE result_of_operation = stackOk_simple(stack);

	char* stack_status = (char*)calloc(25, sizeof(char));

	if (result_of_operation == NULL_STACK_ERROR)
	{
		strcpy(stack_status, "NULL STACK");
	}
	else if (result_of_operation == NULL_BUFFER_ERROR)
	{
		strcpy(stack_status, "NULL BUFFER");
	}
	else if (result_of_operation == ZERO_SIZE_ERROR)
	{
		strcpy(stack_status, "ZERO SIZE");
	}
	else if  (result_of_operation == ZERO_CAPACITY_ERROR)
	{
		strcpy(stack_status, "ZERO CAPACITY");
	}
	else if (result_of_operation == BIG_SIZE_ERROR)
	{
		strcpy(stack_status, "SIZE GREATER THAN CAPACITY");
	}
	else if (result_of_operation == NOLL_ELEMENT_ERROR)
	{
		strcpy(stack_status, "NaN ELEMENT");
	}
	else if (result_of_operation == CHANGE_SOME_DATA_ERROR)
	{
		strcpy(stack_status, "CHANGE SOME DATA");
	}
	else
	{
		strcpy(stack_status, "ok");
	}

	printf("==================================\n");

	printf("Stack(%s) [0x%p] \"%s\"\n", stack_status, stack, stack->var_name);

	if (strcmp(stack_status, "NULL STACK") == 0)
	{
		printf("\nSTACK HAS NULL POINTER\n");

		printf("==================================\n");

		return;	
	}

	printf("{\n    size = %d\n    capacity = %d\n    buffer[0x%p]\n", stack->size, stack->capacity, stack->buffer);

	if (strcmp(stack_status, "NULL BUFFER") == 0)
	{
		printf("\nSTACK BUFFER HAS NULL POINTER\n");

		printf("==================================\n");

		return;
	}

	if (strcmp(stack_status, "\nSIZE GREATER THAN CAPACITY\n") == 0)
	{
		printf("\nSIZE GREATER THAN CAPACITY\n");

		printf("==================================\n");

		return;
	}

	printf("    {\n");
	for (size_t i = 0; i < stack->capacity; ++i)
	{
		if (!isZero(stack->buffer[i] - NOLL))
		{
			printf("        +[%d] = %lg\n", i, stack->buffer[i]);
		}
		else
		{
			printf("         [%d] = NaN\n", i);
		}
	}
	printf("    }\n");
	printf("}\n");


	printf("==================================\n");
}