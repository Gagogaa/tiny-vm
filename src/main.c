#include <stdio.h>
#include <stdbool.h>
#include "vm.h"
#include "common.h"
#include "opcodes.h"


char const program[] = {
    PSH, 5,
    PSH, 6,
    ADD,
    PSH, 0,
    PSH, 5,
    DIV,
    HLT
};


// TODO Prepare this to accept command line args from the user
int
main (int argc, char *argv[])
{
    UNUSED (argc);
    UNUSED (argv);
    Vm *vm = vm_new (program);

    if (vm)
    {
        VmErrorCode error = vm_run (vm);

        if (error)
            printf ("Error: %s\n", vm_error_code_string (error));

        UNUSED (vm_destroy (vm));
    }
    else
        printf ("Unable to initialize vm\n");
}
