// TODO I feel like the CPU error codes are going to give problems if I dont just use the error_code thats on the CPU object
// Or maybe I should fix the drop function some other way...

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "cpu.h"

int const DEFAULT_STACK_SIZE = 4096;

Cpu *
vm_new(int const *program)
{
    if (program == NULL)
        return 0;

    Cpu *cpu = malloc(sizeof (Cpu));

    if (cpu == NULL)
        return 0;

    cpu->stack = malloc(sizeof (int) * DEFAULT_STACK_SIZE);

    if (cpu->stack == NULL)
    {
        free(cpu);
        return 0;
    }

    cpu->instructions = program;
    cpu->running = true;
    cpu->ip = 0;
    cpu->sp = 0;
    cpu->stack[0] = 0; // Clear the first value
    cpu->last_error = CPU_SUCCESS;

    return cpu;
}

Cpu_Error_Code
vm_destroy(Cpu *cpu)
{
    if (cpu == NULL)
        return CPU_NULL_BEFORE_DESTROY;
    else
    {
        Cpu_Error_Code error_code = CPU_SUCCESS;

        if (cpu->stack == NULL)
            error_code = CPU_STACK_NULL_BEFORE_DESTROY;
        else
            free(cpu->stack);

        free(cpu);

        return error_code;
    }
}

Cpu_Error_Code
vm_reset(Cpu *cpu)
{
    if (cpu == NULL)
        return CPU_NULL_BEFORE_RESET;

    cpu->ip = 0;
    cpu->sp = 0;
    cpu->last_error = CPU_SUCCESS;

    return CPU_SUCCESS;
}

void
push(Cpu *cpu, int val)
{
    cpu->stack[cpu->sp++] = val;
}

int
drop(Cpu *cpu)
{
    if (cpu->sp < 0)
    {
        cpu->last_error = CPU_SP_ZERO_BEFORE_DROP;
        return 0;
    }
    return cpu->stack[--cpu->sp];
}

int
fetch(Cpu *cpu)
{
    return cpu->instructions[cpu->ip++];
}

Cpu_Error_Code
vm_step(Cpu *cpu)
{
    if (cpu == NULL)
        return  CPU_NULL_BEFORE_STEP;

    Cpu_Error_Code error_code = CPU_SUCCESS;

    if (cpu->stack == NULL)                  error_code = CPU_STACK_NULL_BEFORE_STEP;
    else if (cpu->instructions == NULL)      error_code = CPU_INSTRUCTIONS_NULL_BEFORE_STEP;
    else if (cpu->sp > DEFAULT_STACK_SIZE)   error_code = CPU_SP_EXCEEDED_MAX_STACK_SIZE;
    else if (cpu->sp < 0)                    error_code = CPU_SP_LESS_THAN_ZERO;
    else if (!cpu->running)                  error_code = CPU_STEP_CALLED_ON_HALTED_CPU;
    else if (cpu->last_error != CPU_SUCCESS) error_code = cpu->last_error;
    else if (error_code != CPU_SUCCESS)
    {
        cpu->running = false;
        cpu->last_error = error_code;
        return error_code;
    }

    switch (fetch(cpu))
    {
    case HLT:
    {
        cpu->running = false;
        break;
    }
    case PSH:
    {
        int val = fetch(cpu);
        push(cpu, val);
        break;
    }
    case POP:
    {
        int val = drop(cpu);
        printf("%d\n", val);
        UNUSED(val);
        break;
    }
    case ADD:
    {
        int a = drop(cpu);
        int b = drop(cpu);
        push(cpu, a + b);
        break;
    }
    case MUL:
    {
        int a = drop(cpu);
        int b = drop(cpu);
        push(cpu, a * b);
        break;
    }
    case DIV:
    {
        int a = drop(cpu);
        int b = drop(cpu);

        if (b == 0)
            error_code = CPU_DIV_BY_ZERO;
        else
            push(cpu, a / b);

        break;
    }
    case SUB:
    {
        int a = drop(cpu);
        int b = drop(cpu);
        push(cpu, a - b);
        break;
    }
    case XOR:
    {
        int a = drop(cpu);
        int b = drop(cpu);
        push(cpu, a ^ b);
        break;
    }
    case AND:
    {
        int a = drop(cpu);
        int b = drop(cpu);
        push(cpu, a & b);
        break;
    }
    case NOT:
    {
        int val = drop(cpu);
        push(cpu, !val);
        break;
    }
    case OR:
    {
        int a = drop(cpu);
        int b = drop(cpu);
        push(cpu, a | b);
        break;
    }
    case LOG:
    {
        printf("%d\n", fetch(cpu));
        break;
    }
    case DUP:
    {
        int val = drop(cpu);
        push(cpu, val);
        push(cpu, val);
        break;
    }
    case ISEQ:
    {
        int a = drop(cpu);
        int b = drop(cpu);
        push(cpu, (a == b) ? 1 : 0);
        break;
    }
    case ISGT:
    {
        int a = drop(cpu);
        int b = drop(cpu);
        push(cpu, (a > b) ? 1 : 0);
        break;
    }
    case ISGE:
    {
        int a = drop(cpu);
        int b = drop(cpu);
        push(cpu, (a >= b) ? 1 : 0);
        break;
    }
    case JMP:
    {
        int loc = fetch(cpu);
        cpu->ip = loc;
        break;
    }
    case JIF:
    {
        int loc = fetch(cpu);
        int cond = drop(cpu);
        if (cond)
            cpu->ip = loc;
        break;
    }
    case NOP:
        break;
    }

    if (error_code != CPU_SUCCESS)
        cpu->last_error = error_code;

    return error_code;
}
