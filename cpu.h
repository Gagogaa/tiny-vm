// Yes right now this is a mess...
#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef enum
{
    // These will store and load things from the heap
    // LOAD, loc -> push var on stack: that will push to the stack the value of the given variable;
    // STORE, loc <- Poped from stack, var that will pop the stack head and save it in the given variable.
    // I may also need to make stack frames
    // CALL // If call has the number of local variables the next function has??? Just something to think about.
    // RET

    // This tells me I should have it do some memory allocation...
    // OUT resource, <count>: output the
    // GET : Gets a resource descriptor
    // IN : read from a file

    HLT, // HLT 0  | HLT              | Stop the vm                                                                |
    PSH, // PSH 1  | PSH              | Push a value on top of the stack                                           |
    POP, // POP 2  | POP              | Pop a value off of the stack                                               |
    DUP, // Duplicate the value on the stack

    ADD, // ADD 3  | ADD              | Pop and add the top two values of the stack pushing the result on top      |
    SUB, // SUB 4  | SUB              | Pop and subtract the top two values of the stack pushing the result on top |
    MUL, // MUL 5  | MUL              | Pop and multiply the top two values of the stack pushing the result on top |
    DIV, // DIV 6  | DIV              | Pop and divide the top two values of the stack pushing the result on top   |

    XOR,
    AND,
    NOT,
    OR,

    // TODO: Check out the order of a and b
    ISEQ, // POPS them off the stack
    ISGT,
    ISGE,

    JMP, // Jumps
    JIF, // Jump if... JIF loc (pops the value off the stack)

    LOG, // LOG 13 | LOG val          | Prints val                                                                 |
    NOP  // NOP 14 | NOP              | No operation                                                               |
} Cpu_Instruction;

typedef enum
{
    CPU_SUCCESS,
    CPU_NULL_BEFORE_DESTROY,
    CPU_STACK_NULL_BEFORE_DESTROY,
    CPU_NULL_BEFORE_RESET,
    CPU_NULL_BEFORE_STEP,
    CPU_STACK_NULL_BEFORE_STEP,
    CPU_INSTRUCTIONS_NULL_BEFORE_STEP,
    CPU_STEP_CALLED_ON_HALTED_CPU,
    CPU_SP_ZERO_BEFORE_DROP,
    CPU_DIV_BY_ZERO,
    CPU_SP_EXCEEDED_MAX_STACK_SIZE,
    CPU_SP_LESS_THAN_ZERO,
    CPU_ERROR_COUNT
} Cpu_Error_Code;

// TODO Why is this included twice?
/* char const *cpu_error_code_strings[CPU_ERROR_COUNT] = */
/* { */
/*         "CPU_SUCCESS", */
/*         "CPU_NULL_BEFORE_DESTROY", */
/*         "CPU_STACK_NULL_BEFORE_DESTROY", */
/*         "CPU_NULL_BEFORE_RESET", */
/*         "CPU_NULL_BEFORE_STEP", */
/*         "CPU_STACK_NULL_BEFORE_STEP", */
/*         "CPU_INSTRUCTIONS_NULL_BEFORE_STEP", */
/*         "CPU_STEP_CALLED_ON_HALTED_CPU", */
/*         "CPU_SP_ZERO_BEFORE_DROP", */
/*         "CPU_DIV_BY_ZERO", */
/*         "CPU_SP_EXCEEDED_MAX_STACK_SIZE", */
/*         "CPU_SP_LESS_THAN_ZERO" */
/* }; */

typedef struct
{
    int const *instructions;
    int *stack;
    int ip;
    int sp;
    Cpu_Error_Code last_error;
    bool running;
} Cpu;

Cpu *
vm_new(int const *program);

Cpu_Error_Code
vm_destroy(Cpu *cpu);

Cpu_Error_Code
vm_step(Cpu *cpu);

Cpu_Error_Code
vm_reset(Cpu *cpu);
