# tiny-vm
My first foray into virtual machines. When I started this project I hand't really programmed in c and I had no idea how virtual machines worked. So
I followed this [tutorial (link now broken)](https://felixangell.com/blog/implementing-a-virtual-machine-in-c/) to get me this far, and think this is
where it comes to an end.


## Compiling tiny-vm
tiny-vm uses meson for its build system. Read more about it and how to install it [here](mesonbuild.com).

Once you have meson installed run the following to build the vm and assembler:
```bash
meson target
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
