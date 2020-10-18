#include "stack.h"

int main()
{   
    //Here is example of operations with securedStack
    Stack stack = {};
    constructStack(&stack, 10);
    
    for (int i = 0; i < 7; ++i)
    {
        pushStack_simple(&stack, i + 1);
    }

    stackDump_simple(&stack, stackOk_simple(&stack), __LINE__, __TIME__);

    elem_t trash = 0;
    for (size_t i = 0; i < 10; ++i)
    {
        eraseStack_simple(&stack);
        stackDump_simple(&stack, stackOk_simple(&stack), __LINE__, __TIME__);

    }

    return 0;
}