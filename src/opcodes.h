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


#define VM_INSTRUCTIONS\
    INSTR (HLT), \
    INSTR (PSH), \
    INSTR (POP), \
    INSTR (DUP), \
    INSTR (ADD), \
    INSTR (SUB), \
    INSTR (MUL), \
    INSTR (DIV), \
    INSTR (XOR), \
    INSTR (AND), \
    INSTR (NOT), \
    INSTR (OR), \
    INSTR (ISEQ), \
    INSTR (ISGT), \
    INSTR (JMP), \
    INSTR (JIF), \
    INSTR (NOP)


#define INSTR(val) val

typedef enum
{
    VM_INSTRUCTIONS
} VmInstruction;

#undef INSTR


char const *
vm_instruction_string (VmInstruction code);
