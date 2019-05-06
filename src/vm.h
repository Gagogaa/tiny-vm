#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>


#define VM_ERROR_CODES \
    ERROR (VM_SUCCESS), \
    ERROR (VM_NULL_BEFORE_DESTROY), \
    ERROR (VM_STACK_NULL_BEFORE_DESTROY), \
    ERROR (VM_NULL_BEFORE_RESET), \
    ERROR (VM_INSTRUCTIONS_NULL_BEFORE_RESET), \
    ERROR (VM_STACK_NULL_BEFORE_RESET), \
    ERROR (VM_NULL_BEFORE_STEP), \
    ERROR (VM_STACK_NULL_BEFORE_STEP), \
    ERROR (VM_INSTRUCTIONS_NULL_BEFORE_STEP), \
    ERROR (VM_STEP_CALLED_AFTER_HALT), \
    ERROR (VM_SP_ZERO_BEFORE_DROP), \
    ERROR (VM_DIV_BY_ZERO), \
    ERROR (VM_SP_EXCEEDED_MAX_STACK_SIZE), \
    ERROR (VM_NULL_ON_RUN), \
    ERROR (VM_ERROR_COUNT)


#define ERROR(val) val

typedef enum
{
    VM_ERROR_CODES
} VmErrorCode;

#undef ERROR


char const *
vm_error_code_string (VmErrorCode code);


typedef struct
{
    char const *instructions;
    char *stack;
    size_t ip;
    size_t sp;
    VmErrorCode last_error;
    bool running;
} Vm;


Vm *
vm_new (char const instructions[]);


VmErrorCode
vm_destroy (Vm *vm);


VmErrorCode
vm_step (Vm *vm);


VmErrorCode
vm_reset (Vm *vm);


VmErrorCode
vm_run (Vm *vm);
