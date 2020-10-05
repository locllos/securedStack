#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#define EPS 1e-9
#define NOLL (-495350)

#define ConstructStack(pseudo_address, start_size)                      \
    constructStack_simple(pseudo_address, start_size);                  \
    (pseudo_address)->var_name = (char*)calloc(64, sizeof(char));       \
    strcpy((pseudo_address)->var_name, #pseudo_address);                \
    strcpy((pseudo_address)->var_name, (pseudo_address)->var_name + 1)

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
    NULL_BUFFER_ERROR,
    ZERO_CAPACITY_ERROR,
    ZERO_SIZE_ERROR,
    BIG_SIZE_ERROR,
    CHANGE_SOME_DATA_ERROR,
    NOLL_ELEMENT_ERROR

} ERROR_MESSAGE;

typedef struct Stack
{
    size_t size;
    size_t capacity;

    elem_t* buffer;

    char* var_name;
    int canarry;
    int check_sum;

} Stack;

typedef struct Stack_s
{
    size_t size;
    size_t capacity;

    elem_t buffer[1];

    char* var_name;
    int check_sum;

} Stack_s;

int checkSum(Stack* stack);

void constructStack_simple(Stack* stack, const size_t start_size);

Stack* newStack_simple(const size_t start_size);

Stack_s* newStack_smart(const size_t start_size);

void pushStack_simple(Stack* stack, const elem_t value);

elem_t topStack_simple(Stack* stack);

void popStack_simple(Stack* stack);

void deleteStack_simple(Stack* stack);

void deleteStack_smart(Stack_s* stack);

ERROR_MESSAGE stackOk_simple(Stack* stack);

void stackDump_simple(Stack* stack);




