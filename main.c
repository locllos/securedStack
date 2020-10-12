#include "stack.h"

int main()
{
    Stack stack = {};
    constructStack(&stack, 10);
    
    for (int i = 0; i < 7; ++i)
    {
        pushStack_simple(&stack, i + 1);
    }

    stack.size = -5;

    stackDump_simple(&stack);


    return 0;
}