# About asdf-jit
LLVM-based esolang JIT compiler & Bitcode compiler.

# About language 'asdf'

asdf is my own esolang, and I know other esolang named "asdf" already exists, but however.

There are 1-bit integer type flag(init value is 1) and 32-bit integer pointer type data. 
While iterating through input code, if interpreter count:

- a -> flag := !flag
- s -> ptr++ if (flag == 1), ptr-- otherwise
- d -> (*ptr)++ if (flag == 1), (*ptr)-- otherwise
- f -> putchar(*ptr) if (flag == 1), (*ptr = getchar()) otherwise

# How to use (I'm sure you don't want to actually use this)

Note that there is Makefile in this repo. It uses clang++ as C++ compiler, but you can change it to something else, but it should support llvm compile. llvm-gcc, for example.

## 1. make

Type `make`. This will generate a.out in same directory.
a.out is asdf JIT repl (unix) executable.

![make](https://raw.githubusercontent.com/g34r/asdf-jit/master/make.png)

Please ignore warnings and do not ask why I linked boost, XD

## 2. run REPL

REPL has few options, such as:

- `toggle_debug` : toggle debug mode or not. In debug mode, REPL will print log to stdout when processing asdf code.
- `toggle_dump` : toggle dump mode or not. In dump mode, REPL will print dump(generated LLVM IR code) to stdou when finisehd processing asdf code.
- `toggle_ir_out` : toggle ir output mode or not. In ir output mode, REPL will write generated LLVM IR code to output.ll in same directory.

We are going to use "toggle_ir_out" option. Simply type toggle_ir_out to REPL, and then type

`dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddfddddddddddddddddddfadddddddddddddddafddfaddddddddddddddddddddddddddddafadafdddddddddddfsddddddddddf`

This is asdf code for printing asdfJIT\n, where \n is linefeed. Of course, you can input another code whatever you want.

![repl](https://raw.githubusercontent.com/g34r/asdf-jit/master/repl.png)

REPL has printed "asdfJIT\n" to stdout and output.ll.

![ir](https://raw.githubusercontent.com/g34r/asdf-jit/master/ir.png)

This is a bit horrible, because this IR code lasts about 1500 lines. But for now, let's ignore about quality of IR code.

## 3. compiling

Now type `make llc`. This uses llc to make object file from LLVM IR code(in this case, output.ll), and use clang++ to generate executable with object file.

After finish, you can run it. (`./output`)

![make_llc](https://raw.githubusercontent.com/g34r/asdf-jit/master/make_llc.png)

It prints asdfJIT\n, and that's what we did in REPL. But one thing different is that this is executable which compiled asdf code to bitcode, not just IR.

## 4. optimization

We did not satisfy with result of REPL llvm ir output, which was 1500-line-lengths.

But we're lucky enough to have ability to optimize what code. Even more, we can do it in second. Just type `make opt`. 

This uses llvm opt tool with -O3 option, and goes thorugh same steps that we did above to generate executable. It would generate `optimised_output.ll, optimised_output.o, optimised_output`.

![opt](https://raw.githubusercontent.com/g34r/asdf-jit/master/opt.png)

This is the whole content of optimised_output.ll. It's just 20 lines, wow. 1500 reduced to 20.
