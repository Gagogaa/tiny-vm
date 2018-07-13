// NOTE C-p in vim is auto complete
#include <stdio.h>
#include <stdbool.h>

typedef enum
{
    PSH,
    ADD,
    POP,
    SET,
    HLT,
    MUL,
    DIV,
    SUB,
    MOV,
    LOG,
    IF,
    IFN,
    GLD,
    GPT,
    NOP
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
val_pop()
{
    return stack[sp--];
}

// TODO maybe this should be called next_instruction (or something like that) instead of having 2 pop methods
int
prg_pop()
{
    return program[++ip];
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
            push(prg_pop());
            break;
        }
        case POP:
        {
            printf("Popped %d\n", val_pop());
            break;
        }
        case ADD:
        {
            int a = val_pop();
            int b = val_pop();
            push(a + b);
            break;
        }
        case SET:
        {
            int reg = val_pop();
            int val = val_pop();
            registers[reg] = val;
            break;
        }
        case MUL:
        {
            int a = val_pop();
            int b = val_pop();
            push(a * b);
            break;
        }
        case DIV:
        {
            int a = val_pop();
            int b = val_pop();
            push(a / b);
            break;
        }
        case SUB:
        {
            int a = val_pop();
            int b = val_pop();
            push(a - b);
            break;
        }
        case MOV:
        {
            int reg_a = prg_pop();
            int reg_b = prg_pop();
            registers[reg_b] = registers[reg_a];
            break;
        }
        case LOG:
        {
            printf("%d\n", prg_pop());
            break;
        }
        // TODO make these work
        case IF:
        {
            break;
        }
        case IFN:
        {
            break;
        }
        case GLD:
        {
            push(registers[prg_pop()]);
            break;
        }
        case GPT:
        {
            int reg = prg_pop();
            registers[reg] = val_pop();
            break;
        }
        case NOP:
            break;
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
