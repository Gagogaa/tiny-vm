#include <stdio.h>
#include <stdbool.h>

typedef enum
{
    PSH,
    ADD,
    POP,
    SET,
    HLT
} InstructionSet;

typedef enum
{
    A, B, C, D, E, F, PC, SP,
    REGISTER_COUNT
} Registers;

#define sp (registers[SP])
#define ip (registers[PC])

int registers[REGISTER_COUNT];
bool running = true;
int stack[4096];

int const program[] = {
    PSH, 5,
    PSH, 6,
    ADD,
    POP,
    HLT
};

void
push(int val)
{
    sp++;
    stack[sp] = val;
}

int
pop()
{
    return stack[sp--];
}

int
fetch()
{
    return program[ip];
}

void
eval(int instr)
{
    switch (instr)
    {
        case HLT:
        {
            running = false;
            break;
        }
        case PSH:
        {
            push(program[++ip]);
            break;
        }
        case POP:
        {
            printf("Popped %d\n", pop());
            break;
        }
        case ADD:
        {
            int a = pop();
            int b = pop();
            push(a + b);
            break;
        }
    }
}

int
main()
{
    while(running)
    {
        eval(fetch());
        ip++;
    }
}
