#include <stdbool.h>
#include <stdlib.h>
#include "common.h"
#include "vm.h"
#include "opcodes.h"


int const DEFAULT_STACK_SIZE = 4096;


#define ERROR(val) #val

char const *vm_error_code_strings[VM_ERROR_COUNT + 1] =
{
    VM_ERROR_CODES
};

#undef ERROR


#define INSTR(val) #val

char const *vm_instruction_strings[NOP + 1] =
{
    VM_INSTRUCTIONS
};

#undef INSTR


char const *
vm_error_code_string (VmErrorCode code)
{
    if (code < 0 || code > VM_ERROR_COUNT)
        return "";
    else
        return vm_error_code_strings[code];
}


char const *
vm_instruction_string (VmInstruction inst)
{
    if (inst < 0 || inst > NOP)
        return "";
    else
        return vm_instruction_strings[inst];
}


// TODO Here I think we can improve this by having a version that
// reads in instructions in from a file
Vm *
vm_new (char const instructions[])
{
    E_CHECK (instructions != NULL, NULL);
    Vm *vm = malloc (sizeof (Vm));
    E_CHECK (vm != NULL, NULL);
    vm->stack = malloc (sizeof (char) * DEFAULT_STACK_SIZE);

    if (vm->stack == NULL)
    {
        free (vm);
        return NULL;
    }

    vm->instructions = instructions;
    vm->running = true;
    vm->ip = 0;
    vm->sp = 0;
    vm->stack[0] = 0; // Clear the first value
    vm->last_error = VM_SUCCESS;

    return vm;
}


VmErrorCode
vm_destroy (Vm *vm)
{
    E_CHECK (vm != NULL, VM_NULL_BEFORE_DESTROY);
    VmErrorCode error_code = VM_SUCCESS;

    if (vm->stack == NULL)
        error_code = VM_STACK_NULL_BEFORE_DESTROY;
    else
        free (vm->stack);

    free (vm);

    return error_code;
}


VmErrorCode
vm_reset (Vm *vm)
{
    E_CHECK (vm != NULL, VM_NULL_BEFORE_RESET);
    E_CHECK (vm->stack != NULL, VM_STACK_NULL_BEFORE_RESET);
    E_CHECK (vm->instructions != NULL, VM_INSTRUCTIONS_NULL_BEFORE_RESET);

    vm->ip = 0;
    vm->sp = 0;
    vm->stack[0] = 0; // Clear the first value
    vm->last_error = VM_SUCCESS;

    return VM_SUCCESS;
}


VmErrorCode
vm_run (Vm *vm)
{
    E_CHECK (vm != NULL, VM_NULL_ON_RUN);
    VmErrorCode error;

    while (vm->running)
    {
        error = vm_step (vm);

        if (error)
            break;
    }

    return error;
}


void
push (Vm *vm, int val)
{
    vm->stack[vm->sp++] = val;
}


int
drop (Vm *vm)
{
    if (vm->sp == 0)
    {
        vm->last_error = VM_SP_ZERO_BEFORE_DROP;
        return 0;
    }

    return vm->stack[--vm->sp];
}


int
fetch (Vm *vm)
{
    return vm->instructions[vm->ip++];
}


VmErrorCode
vm_step (Vm *vm)
{
    E_CHECK (vm != NULL, VM_NULL_BEFORE_STEP);
    E_CHECK (vm->stack != NULL, VM_STACK_NULL_BEFORE_STEP);
    E_CHECK (vm->instructions != NULL, VM_INSTRUCTIONS_NULL_BEFORE_STEP);
    E_CHECK (vm->sp <= DEFAULT_STACK_SIZE, VM_SP_EXCEEDED_MAX_STACK_SIZE);
    E_CHECK (vm->running, VM_STEP_CALLED_AFTER_HALT);
    E_CHECK (vm->last_error == VM_SUCCESS, vm->last_error);

    switch (fetch (vm))
    {
    case HLT:
    {
        vm->running = false;
        break;
    }
    case PSH:
    {
        int val = fetch (vm);
        push (vm, val);
        break;
    }
    case POP:
    {
        int val = drop (vm);
        UNUSED (val);
        break;
    }
    case ADD:
    {
        int a = drop (vm);
        int b = drop (vm);
        push (vm, a + b);
        break;
    }
    case MUL:
    {
        int a = drop (vm);
        int b = drop (vm);
        push (vm, a * b);
        break;
    }
    case DIV:
    {
        int a = drop (vm);
        int b = drop (vm);

        if (b == 0)
            vm->last_error = VM_DIV_BY_ZERO;
        else
            push (vm, a / b);

        break;
    }
    case SUB:
    {
        int a = drop (vm);
        int b = drop (vm);
        push (vm, a - b);
        break;
    }
    case XOR:
    {
        int a = drop (vm);
        int b = drop (vm);
        push (vm, a ^ b);
        break;
    }
    case AND:
    {
        int a = drop (vm);
        int b = drop (vm);
        push (vm, a & b);
        break;
    }
    case NOT:
    {
        int val = drop (vm);
        push (vm, !val);
        break;
    }
    case OR:
    {
        int a = drop (vm);
        int b = drop (vm);
        push (vm, a | b);
        break;
    }
    case DUP:
    {
        int val = drop (vm);
        push (vm, val);
        push (vm, val);
        break;
    }
    case ISEQ:
    {
        int a = drop (vm);
        int b = drop (vm);
        push (vm, (a == b) ? 1 : 0);
        break;
    }
    case ISGT:
    {
        int a = drop (vm);
        int b = drop (vm);
        push (vm, (a > b) ? 1 : 0);
        break;
    }
    case JMP:
    {
        int loc = fetch (vm);
        vm->ip = loc;
        break;
    }
    case JIF:
    {
        int loc = fetch (vm);
        int cond = drop (vm);

        if (cond)
            vm->ip = loc;

        break;
    }
    case NOP:
        break;
    }

    return vm->last_error;
}
