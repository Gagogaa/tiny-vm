#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "common.h"
#include "opcodes.h"


int
main (int argc, char *argv[])
{
    FILE *vm_program;

    if (argc != 2)
    {
        printf ("Vm only accepts one argument: \"vm FILE\"\n");
        exit (-1);
    }

    if (!fopen_s(&vm_program, argv[1], "r"))
    {
        printf ("Vm could not open %s\n", argv[1]);
        exit (-1);
    }


    if (vm_program)
    {
        Vm *vm = vm_new (vm_program);

        if (vm)
        {
            vm->verbose = true;
            VmErrorCode error = vm_run (vm);

            if (error)
                printf ("Error: %s\n", vm_error_code_string (error));

            UNUSED (vm_destroy (vm));
        }
        else
            printf ("Unable to initialize vm\n");

        fclose (vm_program);
    }
    else
        printf ("Error opening file %s\n", argv[1]);
}
