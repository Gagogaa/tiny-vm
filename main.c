// TODO: How to handle vm errors?
// TODO: Maybe have some standard io operations?
// TODO: Maybe some memory related functinos?
// TODO: Load in files while executing. Use threads?
// TODO: Command line arguments.

#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"
#include "common.h"

int const program[] = {
    PSH, 5,
    PSH, 6,
    ADD,
    POP,
    HLT
};

int
main()
{
    Cpu *cpu = vm_new(program);
    if (cpu == NULL)
        return 1;

    while(cpu->running)
    {
        UNUSED(vm_step(cpu));
    }

    UNUSED(vm_destroy(cpu));
}
