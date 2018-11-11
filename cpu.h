#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

// | HLT     | Stop the vm                                                                                      |
// | PSH N   | Push N on top of the stack                                                                       |
// | POP     | Pop a value off of the stack                                                                     |
// | DUP     | Duplicate the value on the stack                                                                 |
// | ADD     | Pop and add the top two values of the stack pushing the result on top                            |
// | SUB     | Pop and subtract the top two values of the stack pushing the result on top                       |
// | MUL     | Pop and multiply the top two values of the stack pushing the result on top                       |
// | DIV     | Pop and divide the top two values of the stack pushing the result on top                         |
// | XOR     | Pop and xor the top two values of the stack pushing the result on top                            |
// | XOR     | Pop and binary and the top two values of the stack pushing the result on top                     |
// | NOT     | Pop and not the top value of the stack pushing the result on top                                 |
// | OR      | Pop and binary or the top two calus of the stack pushing the result on top                       |
// | ISEQ    | Pop off and compare the top two values of the stack pushing the result on top (0 false, 1 true)  |
// | ISGT    | Pop and compair the top two values from the stack pushing the result on top (e.g. is top > next) |
// | JMP loc | Set the ip to loc                                                                                |
// | JIF loc | Pop off the top value and set the ip to loc if the top value was not zero                        |
// | NOP     | No operation                                                                                     |

// OUT resource, <count>: output the
#define CPU_INSTRUCTIONS\
    X (HLT), \
    X (PSH), \
    X (POP), \
    X (DUP), \
    X (ADD), \
    X (SUB), \
    X (MUL), \
    X (DIV), \
    X (XOR), \
    X (AND), \
    X (NOT), \
    X (OR), \
    X (ISEQ), \
    X (ISGT), \
    X (JMP), \
    X (JIF), \
    X (NOP)

#define CPU_ERROR_CODES \
    X (CPU_SUCCESS), \
    X (CPU_NULL_BEFORE_DESTROY), \
    X (CPU_STACK_NULL_BEFORE_DESTROY), \
    X (CPU_NULL_BEFORE_RESET), \
    X (CPU_INSTRUCTIONS_NULL_BEFORE_RESET), \
    X (CPU_STACK_NULL_BEFORE_RESET), \
    X (CPU_NULL_BEFORE_STEP), \
    X (CPU_STACK_NULL_BEFORE_STEP), \
    X (CPU_INSTRUCTIONS_NULL_BEFORE_STEP), \
    X (CPU_STEP_CALLED_ON_HALTED_CPU), \
    X (CPU_SP_ZERO_BEFORE_DROP), \
    X (CPU_DIV_BY_ZERO), \
    X (CPU_SP_EXCEEDED_MAX_STACK_SIZE), \
    X (CPU_ERROR_COUNT)

#define X(val) val

typedef enum
{
    CPU_INSTRUCTIONS
} Cpu_Instruction;

typedef enum
{
    CPU_ERROR_CODES
} Cpu_Error_Code;

#undef X

char const *
cpu_error_code_string (Cpu_Error_Code code);

char const *
cpu_instruction_string (Cpu_Instruction code);

typedef struct
{
    char const *instructions;
    char *stack;
    size_t ip;
    size_t sp;
    Cpu_Error_Code last_error;
    bool running;
} Cpu;

Cpu *
vm_new (char const *instructions);

Cpu_Error_Code
vm_destroy (Cpu *cpu);

Cpu_Error_Code
vm_step (Cpu *cpu);

Cpu_Error_Code
vm_reset (Cpu *cpu);
