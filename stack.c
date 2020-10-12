#include "stack.h"

#define DEBUG

#ifdef DEBUG
#define ASSERT_OK(stack)							\
    if(stackOk_simple(stack) != NO_ERROR)			\
    {												\
        stackDump_simple(stack);					\
        assert(0);									\
    }
#else ASSERT_OK(stack) {}
#endif
    
int isZero(elem_t value)
{
    return (fabs(value) < EPS);
}

int hashSum(const void* buffer, const int length)
{
    const int heshy = 15487457 * 23;
    int hash_sum = 0;

    for (int i = 0; i < length; ++i)
    {
        hash_sum += ((int)((char*)buffer + i) * (i * (i + 1))) % heshy;

        if (i % 100)
        {
            hash_sum %= hash_sum & heshy;
        }
    }

    return hash_sum;
}

int checkSum(Stack* stack)
{
    const int heshy = 15487457 * 23;

    int check_sum = (int)stack;
    check_sum += (int)stack->capacity;
    check_sum += (int)stack->size;

    check_sum += hashSum(stack->buffer, stack->size * sizeof(elem_t));

    for (int i = 0; i < stack->size; ++i)
    {
        check_sum += stack->buffer[i] * (i + 1);

        if (i % 5 == 0)
        {
            check_sum %= heshy;
        }
    }

    check_sum += (int)stack->buffer;

    return check_sum;
}

int sizeStack_simple(const Stack* stack)
{
    ASSERT_OK(stack);
    return stack->size;
    ASSERT_OK(stack);
}

void fillNolls(elem_t* start, elem_t* end)
{
    for (int i = 0; i < end - start + 1; ++i)
    {
        start[i] = NOLL;
    }
}

void constructStack_simple(Stack* stack, int start_capacity, const char* var_name)
{   
    if (start_capacity == 0)
    {
        start_capacity = 1;
    }
    else if (start_capacity < 0)
    {   
        stack = NULL;
        stackDump_simple(stack);
        assert(!"NEGATIVE START CAPACITY");
    }

    //Fill buffer canarries
    stack->buffer_canarry_a = (long int*)calloc(1, sizeof(elem_t) * start_capacity + sizeof(long int) * 2);

    assert(stack->buffer_canarry_a != NULL);

    stack->buffer = (elem_t*)((char*)stack->buffer_canarry_a + sizeof(long int));
    stack->buffer_canarry_b = (long int*)((char*)stack->buffer + sizeof(elem_t) * start_capacity);
    //Fill stack canarries
    stack->stack_canarry_a = (long int*)stack;
    stack->stack_canarry_b = (long int*)stack;
    
    stack->capacity = start_capacity;
    stack->size = 0;  
    stack->var_name = var_name + 1;  

    fillNolls(stack->buffer + stack->size, stack->buffer + stack->capacity);

    stack->check_sum = checkSum(stack);

    ASSERT_OK(stack);

}

Stack* newStack_simple(const int start_capacity)
{
    char* var_name = (char*)calloc(72, sizeof(char));
    strcpy(var_name, "<Name of variable is not available when called newStack_simple>");

    Stack* new_stack = (Stack*)calloc(1, sizeof(Stack));

    new_stack->stack_canarry_a = (long int*)new_stack;
    new_stack->stack_canarry_b = (long int*)new_stack;

    constructStack_simple(new_stack, start_capacity, var_name);

    ASSERT_OK(new_stack);

    free(var_name);
    return new_stack;
}

void changeCapacity(Stack* stack, const double factor, const OPERATION operation)
{   
    if (operation == PUSH)
    {   
        stack->capacity = factor * stack->capacity;
    }
    else if (operation == POP)
    {
        stack->capacity = (int)(stack->capacity / factor);
    }
    else
    {
        return;
    }

    stack->buffer_canarry_a = (long int*)realloc(stack->buffer_canarry_a, sizeof(elem_t) * stack->capacity + sizeof(long int) * 2);

    assert(stack->buffer_canarry_a != NULL);

    stack->buffer = (elem_t*)((char*)stack->buffer_canarry_a + sizeof(long int));
    stack->buffer_canarry_b = (long int*)((char*)stack->buffer + sizeof(elem_t) * stack->capacity);
}

void checkSizeCapacity(Stack* stack, const OPERATION operation)
{
    if (stack->size + 1 > stack->capacity && operation == PUSH)
    {
        changeCapacity(stack, 2, operation);
    }
    else if (stack->size < stack->capacity / 4 && operation == POP)
    {
        changeCapacity(stack, 4, operation);

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
    if (stack->size == 0)
    {
        stackDump_simple(stack);
        assert(!"EMPTY STACK");
    }

    return stack->buffer[stack->size - 1];

}

void popStack_simple(Stack* stack)
{
    ASSERT_OK(stack);
    if (stack->size == 0)
        return;

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

ERROR_MESSAGE stackOk_simple(Stack* stack)
{
    static int count = 0;
    count++;

    if (stack == NULL)
    {
        return NULL_STACK_ERROR;
    }
    else if (stack->stack_canarry_a != (long int*)stack)
    {
        return STACK_CANARRY_A_ERROR;
    }
    else if (stack->stack_canarry_b != (long int*)stack)
    {
        return STACK_CANARRY_B_ERROR;
    }
    else if (stack->buffer == NULL)
    {
        return NULL_BUFFER_ERROR;
    }
    else if (stack->buffer_canarry_a != (long int*)((char*)stack->buffer - sizeof(long int)))
    {
        return BUFFER_CANARRY_A_ERROR;
    }
    else if (stack->buffer_canarry_b != (long int*)((char*)stack->buffer + sizeof(elem_t) * stack->capacity))
    {
        return BUFFER_CANARRY_B_ERROR;
    }
    else if (stack->size <= 0 && stack->capacity <= 0)
    {
        return BAD_SIZE_ERROR;
    }
    else if (stack->size < 0)
    {
        return BAD_SIZE_ERROR;
    }
    else if (stack->capacity < 1)
    {
        return BAD_CAPACITY_ERROR;
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

    for (int i = 0; i < stack->size; ++i)
    {
        if (isnan(stack->buffer[i]))
        {
            return NOLL_ELEMENT_ERROR;
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
    else if (result_of_operation == STACK_CANARRY_A_ERROR)
    {
        strcpy(stack_status, "STACK CANARRY A ERROR");
    }
    else if (result_of_operation == STACK_CANARRY_B_ERROR)
    {
        strcpy(stack_status, "STACK CANARRY B ERROR");
    }
    else if (result_of_operation == NULL_BUFFER_ERROR)
    {
        strcpy(stack_status, "NULL BUFFER");
    }
    else if (result_of_operation == BUFFER_CANARRY_A_ERROR)
    {
        strcpy(stack_status, "BUFFER CANARRY A ERROR");
    }
    else if (result_of_operation == BUFFER_CANARRY_B_ERROR)
    {
        strcpy(stack_status, "BUFFER CANARRY B ERROR");
    }
    else if (result_of_operation == BAD_SIZE_ERROR)
    {
        strcpy(stack_status, "BAD SIZE AND CAPACITY");
    }
    else if  (result_of_operation == BAD_CAPACITY_ERROR)
    {
        strcpy(stack_status, "BAD CAPACITY");
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

    if (strcmp(stack_status, "NULL STACK") == 0)
    {
        printf("\nSTACK HAS NULL POINTER\n");

        printf("==================================\n");

        return;
    }

    printf("Stack(%s) [0x%p] \"%s\"\n", stack_status, stack, stack->var_name);

    printf("{\n\tsize = %d\n\tcapacity = %d\n\tbuffer[0x%p]"
            "\n\tstack_canarry_a = 0x%p\n\tstack_canarry_b = 0x%p", 
            stack->size, stack->capacity, stack->buffer, stack->stack_canarry_a, stack->stack_canarry_b);

    if (strcmp(stack_status, "NULL BUFFER") == 0)
    {
        printf("\nSTACK BUFFER HAS NULL POINTER\n");

        printf("==================================\n");

        return;
    }

    printf("\n\tbuffer_canarry_a = 0x%p\n\tbuffer_canarry_b = 0x%p\n", 
            stack->buffer_canarry_a, stack->buffer_canarry_b);

    if (result_of_operation == BIG_SIZE_ERROR)
    {
        printf("\nSIZE GREATER THAN CAPACITY\n");

        printf("==================================\n");

        return;
    }

    printf("\t{\n");
    for (int i = 0; i < stack->capacity; ++i)
    {
        if (!isnan(stack->buffer[i]))
        {
            printf("\t\t+[%d] = %lg\n", i, stack->buffer[i]);
        }
        else
        {
            printf("\t\t [%d] = NaN\n", i);
        }
    }
    printf("\t}\n");
    printf("}\n");


    printf("==================================\n");
}