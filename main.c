// TODO: How to handle vm errors?
// TODO: Maybe have some standard out operations?
// TODO: Maybe some memory related functinos?
// TODO: Load in files while executing. Use threads?
// TODO: Command line arguments.

#include <stdio.h>
#include <stdbool.h>

typedef enum
{
    HLT_INS, // HLT 0  | HLT              | Stop the vm                                                                |
    PSH_INS, // PSH 1  | PSH              | Push a value on top of the stack                                           |
    POP_INS, // POP 2  | POP              | Pop a value off of the stack                                               |
    ADD_INS, // ADD 3  | ADD              | Pop and add the top two values of the stack pushing the result on top      |
    SUB_INS, // SUB 4  | SUB              | Pop and subtract the top two values of the stack pushing the result on top |
    MUL_INS, // MUL 5  | MUL              | Pop and multiply the top two values of the stack pushing the result on top |
    DIV_INS, // DIV 6  | DIV              | Pop and divide the top two values of the stack pushing the result on top   |
    SET_INS, // SET 7  | SET reg, val     | Set the value of reg to val                                                |
    MOV_INS, // MOV 8  | MOV reg_a, reg_b | Move the value in reg_a to reg_b                                           |
     IF_INS, //  IF 9  |  IF reg, val, ip | If reg == val set the instruction pointer to ip                            |
    IFN_INS, // IFN 10 | IFN reg, val, ip | If reg != val set teh instruction pointer to ip                            |
    GLD_INS, // GLD 11 | GLD reg          | Pushes the value in reg on top the the stack                               |
    GPT_INS, // GPT 12 | GPT reg          | Pushes teh top of the stack into reg                                       |
    LOG_INS, // LOG 13 | LOG val          | Prints val                                                                 |
    NOP_INS  // NOP 14 | NOP              | No operation                                                               |
} InstructionSet;

typedef enum
{
    A, B, C, D, E, F, G, H,
    // ECX, // Loop count
    IP,
    SP,
    REGISTER_COUNT
} Registers;

int registers[REGISTER_COUNT];
bool running = true;
int stack[4096]; // TODO Maybe needs to resize itself?

// A better version of the program for loading things in
// int *instructions

int const program[] = {
    PSH_INS, 5,
    PSH_INS, 6,
    ADD_INS,
    POP_INS,
    HLT_INS
};

#define TARGET(op) \
    /* TARGET_##op: lets you jump to the case statment? */ \
    case op:

#define PUSH(val) (stack[(registers[SP])++] = (val))
#define POP() (stack[--(registers[SP])])
#define FETCH() (program[(registers[IP])++])

void
eval(int instr)
{
    switch (instr)
    {
        TARGET(HLT_INS)
        {
            running = false;
            break;
        }
        TARGET(PSH_INS)
        {
            int val = FETCH();
            PUSH(val);
            break;
        }
        TARGET(POP_INS)
        {
            int val = POP();
            printf("Popped %d\n", val);
            break;
        }
        TARGET(ADD_INS)
        {
            int a = POP();
            int b = POP();
            PUSH(a + b);
            break;
        }
        TARGET(SET_INS)
        {
            int reg = POP();
            int val = POP();
            registers[reg] = val;
            break;
        }
        TARGET(MUL_INS)
        {
            int a = POP();
            int b = POP();
            PUSH(a * b);
            break;
        }
        TARGET(DIV_INS)
        {
            int a = POP();
            int b = POP();
            PUSH(a / b); // What to do if divide by zero?
            break;
        }
        TARGET(SUB_INS)
        {
            int a = POP();
            int b = POP();
            PUSH(a - b);
            break;
        }
        TARGET(MOV_INS)
        {
            int reg_a = FETCH();
            int reg_b = FETCH();
            registers[reg_b] = registers[reg_a];
            break;
        }
        TARGET(LOG_INS)
        {
            printf("%d\n", FETCH());
            break;
        }
        TARGET(IF_INS)
        {
            int reg = FETCH();
            int val = FETCH();
            int jmp_loc = FETCH();
            if (registers[reg] == val)
                (registers[IP]) = jmp_loc;
            break;
        }
        TARGET(IFN_INS)
        {
            int reg = FETCH();
            int val = FETCH();
            int jmp_loc = FETCH();
            if (registers[reg] != val)
                (registers[IP]) = jmp_loc;
            break;
        }
        TARGET(GLD_INS)
        {
            int reg = FETCH();
            int val = registers[reg];
            PUSH(val);
            break;
        }
        TARGET(GPT_INS)
        {
            int reg = FETCH();
            int val = POP();
            registers[reg] = val;
            break;
        }
        TARGET(NOP_INS)
            break;
    }
}

int
main()
{
    while(running)
    {
        eval(FETCH());
    }
}
