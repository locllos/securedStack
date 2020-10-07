#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#define EPS 1e-9
#define NOLL NAN

#define сonstructStack(pseudo_address, start_size)                      \
    char* var_name = (char*)calloc(72, sizeof(char));                   \
    strcpy(var_name, #pseudo_address);                                  \
    constructStack_simple(pseudo_address, start_size, var_name + 1);    \
    free(var_name)                                                  


typedef double elem_t;

typedef enum OPERATION
{
    PUSH,
    POP

} OPERATION;

typedef enum ERROR_MESSAGE
{
    NO_ERROR,

    NULL_STACK_ERROR,
    STACK_CANARRY_A_ERROR,
    STACK_CANARRY_B_ERROR,
    BUFFER_CANARRY_A_ERROR,
    BUFFER_CANARRY_B_ERROR,
    NULL_BUFFER_ERROR,
    ZERO_CAPACITY_ERROR,
    ZERO_SIZE_ERROR,
    BIG_SIZE_ERROR,
    CHANGE_SOME_DATA_ERROR,
    NOLL_ELEMENT_ERROR

} ERROR_MESSAGE;

typedef struct Stack
{
    long int* stack_canarry_a;

    size_t size;
    size_t capacity;
    elem_t* buffer;
    char* var_name;
    int check_sum;
    
    long int* buffer_canarry_a;
    long int* buffer_canarry_b;

    long int* stack_canarry_b;

} Stack;

int checkSum(Stack* stack);

void constructStack_simple(Stack* stack, const size_t start_capacity, const char* var_name);

Stack* newStack_simple(const size_t start_capacity);

void pushStack_simple(Stack* stack, const elem_t value);

elem_t topStack_simple(Stack* stack);

void popStack_simple(Stack* stack);

void deleteStack_simple(Stack* stack);

ERROR_MESSAGE stackOk_simple(Stack* stack);

void stackDump_simple(Stack* stack);



