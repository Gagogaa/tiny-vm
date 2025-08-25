#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>


#define VM_ERROR_CODES                      \
    ERROR (VM_SUCCESS),                     \
    ERROR (VM_STACK_IS_NULL),               \
    ERROR (VM_IS_NULL),                     \
    ERROR (VM_PROGRAM_IS_NULL),             \
    ERROR (VM_PROGRAM_EOF_ON_STEP),         \
    ERROR (VM_SP_ZERO_BEFORE_DROP),         \
    ERROR (VM_DIV_BY_ZERO),                 \
    ERROR (VM_SP_EXCEEDED_MAX_STACK_SIZE),  \
    ERROR (VM_NOT_RUNNING_ON_STEP),         \
    ERROR (VM_STACK_SIZE_IS_ZERO),          \
    ERROR (VM_ERROR_COUNT) // Sential value. Keep this at the end.


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
    FILE *program;
    size_t stack_size;
    char *stack;
    size_t sp;
    VmErrorCode last_error;
    bool running;
    bool verbose;
} Vm;


VmErrorCode
vm_init (Vm *vm, char *stack, size_t stack_size, FILE *program);


Vm *
vm_new (FILE *program);


VmErrorCode
vm_destroy (Vm *vm);


VmErrorCode
vm_step (Vm *vm);


VmErrorCode
vm_reset (Vm *vm);


VmErrorCode
vm_run (Vm *vm);
