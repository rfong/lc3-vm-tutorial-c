This is example C scaffolding and toolchain setup guidance for following 
[jmeiners' LC3 VM tutorial](https://www.jmeiners.com/lc3-vm/), 
set up in a way that I think is easier to follow. The original tutorial 
puts a lot of setup code near the end, so it can be hard to check your 
progress partway through.

The tutorial's enums, setup code, and memory management are provided so you 
don't have to copypasta everything in from the tutorial. Just instruction set 
implementations are left as an exercise for the reader. However, I still 
recommend going through all the scaffolding code line by line to get the 
educational benefits.

# How to

## Setup
Install `gcc`. You will use `gcc` to compile your C file into an executable.
- MacOS: `brew install gcc`
- Unix: `sudo apt-get install gcc`

## Optional setup: assembler
If you want to be able to write any of your own intermediate test cases in 
Assembly, set up the 
[LC3 tools](https://highered.mheducation.com/sites/0072467509/student_view0/lc-3_simulator.html).

`tar -xvf lc3tools_v12.zip` (or your unzip method of choice)

Follow the instructions in `lc3tools/README.md`. After you do this, you should 
be able to run the assembler on an assembly file, like so:

`lc3as roms/hello.asm`

## Emulate a rom
Assembly files and assembled roms live in `roms/`.

To run a rom on the VM, pass the `.obj` file path to the VM executable.

`./lc3-vm roms/hello.obj`

If you have not implemented anything yet, you should see an error message 
about an op not being implemented.

If you installed the LC3 tools, you can use my Make targets as shorthand to 
assemble new roms. For example, to reassemble and run `roms/hello.asm`, do 
`make run rom=roms/hello`.

## Compile the VM

When you make changes to the LC3 implementation, you'll want to recompile the 
C code to an executable.

You can use `make compile` as shorthand. To see what it does, read `Makefile`.
