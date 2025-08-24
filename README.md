# tiny-vm
My first foray into virtual machines. When I started this project I hand't really programmed in c and I had no idea how virtual machines worked. So
I followed this [tutorial](https://blog.felixangell.com/virtual-machine-in-c) to get me this far, and think this is
where it comes to an end.


## Compiling tiny-vm
tiny-vm uses meson for its build system. Read more about it and how to install it [here](https://mesonbuild.com).

Once you have meson installed run the following to build the vm and assembler:
```bash
meson setup target
ninja -C target
```

## Createing tiny-vm programs
First create a tiny vm program:
```bash
echo "PSH 5
PSH 6
ADD
PSH 0
PSH 5
DIV
HLT" > vm-example.ty
```


Then compile and run the tiny-vm program:
```bash
./target/as -i vm-example.ty -o vm-example.tyc
./target/vm vm-example.tyc
```


## VM-Instructions
For reference here are the vm instructions:

| Instruction | Description                                                                                      |
| ---         | ---                                                                                              |
| HLT         | Stop the vm                                                                                      |
| PSH N       | Push N on top of the stack                                                                       |
| POP         | Pop a value off of the stack                                                                     |
| DUP         | Duplicate the value on the stack                                                                 |
| ADD         | Pop and add the top two values of the stack pushing the result on top                            |
| SUB         | Pop and subtract the top two values of the stack pushing the result on top                       |
| MUL         | Pop and multiply the top two values of the stack pushing the result on top                       |
| DIV         | Pop and divide the top two values of the stack pushing the result on top                         |
| XOR         | Pop and xor the top two values of the stack pushing the result on top                            |
| XOR         | Pop and binary and the top two values of the stack pushing the result on top                     |
| NOT         | Pop and not the top value of the stack pushing the result on top                                 |
| OR          | Pop and binary or the top two calus of the stack pushing the result on top                       |
| ISEQ        | Pop off and compare the top two values of the stack pushing the result on top (0 false, 1 true)  |
| ISGT        | Pop and compair the top two values from the stack pushing the result on top (e.g. is top > next) |
| JMP loc     | Set the ip to loc                                                                                |
| JIF loc     | Pop off the top value and set the ip to loc if the top value was not zero                        |
| NOP         | No operation                                                                                     |
