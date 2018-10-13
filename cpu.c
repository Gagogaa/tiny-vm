#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "cpu.h"

int const DEFAULT_STACK_SIZE = 4096;

#define X(val) #val

char const *cpu_error_code_strings[CPU_ERROR_COUNT + 1] =
{
    CPU_ERROR_CODES
};

char const *cpu_instruction_strings[NOP + 1] =
{
    CPU_INSTRUCTIONS
};

#undef X

char const *
cpu_error_code_string(Cpu_Error_Code code)
{
    if (code < 0 || code > CPU_ERROR_COUNT)
        return "";
    else
        return cpu_error_code_strings[code];
}

char const *
cpu_instruction_string(Cpu_Instruction inst)
{
    if (inst < 0 || inst > NOP)
        return "";
    else
        return cpu_instruction_strings[inst];
}

Cpu *
vm_new(char const *instructions)
{
    ASSERT(instructions != NULL, NULL);

    Cpu *cpu = malloc(sizeof (Cpu));

    ASSERT(cpu != NULL, NULL);

    cpu->stack = malloc(sizeof (char) * DEFAULT_STACK_SIZE);

    if (cpu->stack == NULL)
    {
        free(cpu);
        return NULL;
    }

    cpu->instructions = instructions;
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
    ASSERT(cpu != NULL, CPU_NULL_BEFORE_DESTROY);

    Cpu_Error_Code error_code = CPU_SUCCESS;

    if (cpu->stack == NULL)
        error_code = CPU_STACK_NULL_BEFORE_DESTROY;
    else
        free(cpu->stack);

    free(cpu);

    return error_code;
}

Cpu_Error_Code
vm_reset(Cpu *cpu)
{
    ASSERT(cpu != NULL, CPU_NULL_BEFORE_RESET);
    ASSERT(cpu->stack != NULL, CPU_STACK_NULL_BEFORE_RESET);
    ASSERT(cpu->instructions != NULL, CPU_INSTRUCTIONS_NULL_BEFORE_RESET);

    cpu->ip = 0;
    cpu->sp = 0;
    cpu->stack[0] = 0; // Clear the first value
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
    if (cpu->sp == 0)
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
    ASSERT(cpu != NULL, CPU_NULL_BEFORE_STEP);
    ASSERT(cpu->stack != NULL, CPU_STACK_NULL_BEFORE_STEP);
    ASSERT(cpu->instructions != NULL, CPU_INSTRUCTIONS_NULL_BEFORE_STEP);
    ASSERT(cpu->sp <= DEFAULT_STACK_SIZE, CPU_SP_EXCEEDED_MAX_STACK_SIZE);
    ASSERT(cpu->running, CPU_STEP_CALLED_ON_HALTED_CPU);
    ASSERT(cpu->last_error == CPU_SUCCESS, cpu->last_error);

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
            cpu->last_error = CPU_DIV_BY_ZERO;
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

    return cpu->last_error;
}
