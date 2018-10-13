#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"
#include "common.h"

char const program[] = {
    PSH, 5,
    PSH, 6,
    ADD,
    PSH, 0,
    PSH, 5,
    DIV,
    HLT
};

int
main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    Cpu *cpu = vm_new(program);

    ASSERT(cpu != NULL, 1);

    while (cpu->running)
    {
        Cpu_Error_Code error = vm_step(cpu);

        if (error)
        {
            printf("Error: %s\n", cpu_error_code_string(error));
            break;
        }
    }

    UNUSED(vm_destroy(cpu));
}
