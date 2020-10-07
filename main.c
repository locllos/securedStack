#include "stack.h"

int main()
{
    Stack stack = {};

    сonstructStack(&stack, 1);
    
    for (int i = 0; i < 100; ++i)
    {
        pushStack_simple(&stack, i*i);
    }

    stackDump_simple(&stack);

    for (int i = 0; i < 30; ++i)
    {
        popStack_simple(&stack);
    }
   

    pushStack_simple(&stack, 45);

    stack.buffer[1] = 13;

    stackDump_simple(&stack);
    

    return 0;
}

