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


Vm *
vm_new (FILE *program)
{
    E_CHECK (program != NULL, NULL);
    Vm *vm = malloc (sizeof (Vm));
    E_CHECK (vm != NULL, NULL);
    char *stack = malloc (sizeof (char) * DEFAULT_STACK_SIZE);

    if (stack == NULL)
    {
        free (vm);
        return NULL;
    }

    if (vm_init(vm, stack, (sizeof (char) * DEFAULT_STACK_SIZE), program) != VM_SUCCESS)
    {   
        free (vm);
        free (stack);
        return NULL;
    }

    return vm;
}


VmErrorCode
vm_init (Vm *vm, char *stack, size_t stack_size, FILE *program)
{
    E_CHECK (program != NULL, VM_PROGRAM_IS_NULL);
    E_CHECK (vm != NULL, VM_IS_NULL);
    E_CHECK (stack != NULL, VM_STACK_IS_NULL);
    E_CHECK (stack_size > 0, VM_STACK_SIZE_IS_ZERO);

    vm->stack = stack;
    vm->stack_size = stack_size;
    vm->program = program;
    vm->running = true;
    vm->verbose = false;
    vm->sp = 0;
    vm->stack[0] = 0; // Clear the first value
    vm->last_error = VM_SUCCESS;

    return VM_SUCCESS;
}


VmErrorCode
vm_destroy (Vm *vm)
{
    E_CHECK (vm != NULL, VM_IS_NULL);
    E_CHECK (vm->stack != NULL, VM_STACK_IS_NULL);
    free (vm->stack);
    free (vm);

    return VM_SUCCESS;
}


VmErrorCode
vm_reset (Vm *vm)
{
    E_CHECK (vm != NULL, VM_IS_NULL);
    E_CHECK (vm->stack != NULL, VM_STACK_IS_NULL);
    E_CHECK (vm->program != NULL, VM_PROGRAM_IS_NULL);
    vm->sp = 0;
    vm->stack[0] = 0; // Clear the first value
    vm->last_error = VM_SUCCESS;

    return VM_SUCCESS;
}


VmErrorCode
vm_run (Vm *vm)
{
    E_CHECK (vm != NULL, VM_IS_NULL);
    VmErrorCode error;

    do
    {
        error = vm_step (vm);

        if (error)
            break;
    } while (vm->running);

    return error;
}


void
push (Vm *vm, int val)
{
    vm->stack[vm->sp++] = (char) val;
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
    int c = fgetc (vm->program);

    if (c == EOF)
        vm->last_error = VM_PROGRAM_EOF_ON_STEP;

    return c;
}


void
seek (Vm *vm, int loc)
{
    fseek (vm->program, loc, SEEK_SET);
}


VmErrorCode
vm_step (Vm *vm)
{
    E_CHECK (vm != NULL, VM_IS_NULL);
    E_CHECK (vm->stack != NULL, VM_STACK_IS_NULL);
    E_CHECK (vm->program != NULL, VM_PROGRAM_IS_NULL);
    E_CHECK (vm->sp <= vm->stack_size, VM_SP_EXCEEDED_MAX_STACK_SIZE);
    E_CHECK (vm->running, VM_NOT_RUNNING_ON_STEP);
    E_CHECK (vm->last_error == VM_SUCCESS, vm->last_error);

    int op = fetch (vm);

    if (vm->verbose)
        printf ("%s\n", vm_instruction_string (op));

    switch (op)
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
        seek (vm, loc);
        break;
    }
    case JIF:
    {
        int loc = fetch (vm);
        int cond = drop (vm);

        if (cond)
            seek (vm, loc);

        break;
    }
    case NOP:
        break;
    }

    return vm->last_error;
}
