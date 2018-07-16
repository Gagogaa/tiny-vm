#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "cpu.h"

int const DEFAULT_STACK_SIZE = 4096;
#define TARGET(op) \
    case op:

Cpu *
vm_new()
{
    Cpu *cpu = malloc(sizeof (Cpu));

    if (cpu == NULL)
        return 0;

    cpu->stack = malloc(sizeof (int) * DEFAULT_STACK_SIZE);

    if (cpu->stack == NULL)
    {
        free(cpu);
        return 0;
    }

    cpu->running = true;
    cpu->ip = 0;
    cpu->sp = 0;

    return cpu;
}

void
vm_destroy(Cpu *cpu)
{
    free(cpu->stack);
    free(cpu);
}

void
vm_reset(Cpu *cpu)
{
    cpu->ip = 0;
    cpu->sp = 0;
}

void
push(Cpu *cpu, int val)
{
    cpu->stack[cpu->sp++] = val;
}

int
drop(Cpu *cpu)
{
    return cpu->stack[--cpu->sp];
}

int
fetch(Cpu *cpu)
{
    return cpu->instructions[cpu->ip++];
}

void
vm_step(Cpu *cpu)
{
    switch (fetch(cpu))
    {
        TARGET(HLT)
        {
            cpu->running = false;
            break;
        }
        TARGET(PSH)
        {
            int val = fetch(cpu);
            push(cpu, val);
            break;
        }
        TARGET(POP)
        {
            int val = drop(cpu);
            printf("%d\n", val);
            UNUSED(val);
            break;
        }
        TARGET(ADD)
        {
            int a = drop(cpu);
            int b = drop(cpu);
            push(cpu, a + b);
            break;
        }
        TARGET(MUL)
        {
            int a = drop(cpu);
            int b = drop(cpu);
            push(cpu, a * b);
            break;
        }
        TARGET(DIV)
        {
            int a = drop(cpu);
            int b = drop(cpu);
            push(cpu, a / b); // What to do if divide by zero?
            break;
        }
        TARGET(SUB)
        {
            int a = drop(cpu);
            int b = drop(cpu);
            push(cpu, a - b);
            break;
        }
        TARGET(XOR)
        {
            int a = drop(cpu);
            int b = drop(cpu);
            push(cpu, a ^ b);
            break;
        }
        TARGET(AND)
        {
            int a = drop(cpu);
            int b = drop(cpu);
            push(cpu, a & b);
            break;
        }
        TARGET(NOT)
        {
            int val = drop(cpu);
            push(cpu, !val);
            break;
        }
        TARGET(OR)
        {
            int a = drop(cpu);
            int b = drop(cpu);
            push(cpu, a | b);
            break;
        }
        TARGET(LOG)
        {
            printf("%d\n", fetch(cpu));
            break;
        }
        TARGET(DUP){
            int val = drop(cpu);
            push(cpu, val);
            push(cpu, val);
            break;
        }
        TARGET(ISEQ)
        {
            int a = drop(cpu);
            int b = drop(cpu);
            push(cpu, (a == b) ? 1 : 0);
            break;
        }
        TARGET(ISGT)
        {
            int a = drop(cpu);
            int b = drop(cpu);
            push(cpu, (a > b) ? 1 : 0);
            break;
        }
        TARGET(ISGE)
        {
            int a = drop(cpu);
            int b = drop(cpu);
            push(cpu, (a >= b) ? 1 : 0);
            break;
        }
        TARGET(JMP)
        {
            int loc = fetch(cpu);
            cpu->ip = loc;
            break;
        }
        TARGET(JIF)
        {
            int loc = fetch(cpu);
            int cond = drop(cpu);
            if (cond)
                cpu->ip = loc;
            break;
        }
        TARGET(NOP)
            break;
    }
}
