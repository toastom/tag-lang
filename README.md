## tag

---

### What is this?

Welcome! This is a repo for a completely unserious and unimpressive hobby programming language and compiler. The working name for it right now is "tag," mainly because it's my language and those are my initials and I'm not all that creative. 

### Why?

The project was inspired by and based off of Pixeled's Youtube series documenting the development of his compiler for a language called Hydrogen:

- [Pixeled's Creating a Compiler series](https://www.youtube.com/playlist?list=PLUDlas_Zy_qC7c5tCgTMYq2idyyT241qs)
- [hydrogen language](https://github.com/orosmatthew/hydrogen-cpp)

Much of the initial code was ripped from him, which I then translated into C from C++.

I decided to do my own version because it seemed like a fun way to learn how compilers work. The tag compiler is written in C for the same reason, because it's for fun I wanted to learn more. My C is also a little rusty so I wanted to practice it more on a fun project.

Checkout the `docs/` folder for current tag language `grammar` and future `syntax` ideas. Everything in there is subject to change.

---
### Build

Building the compiler assumes an x86_64 Linux system with `gcc` and `nasm` installed. This project is perhaps the easiest possible install. Literally just

`git clone <project url>`

`cd <path-to-project>`

`gcc main.c Tokenizer.c Parser.c Generation.c -o tag`

then create a tag source code file to compile!

`nano helloworld.tag`

Inside this source file, insert a simple "hello, world" script:
```
disp("Hello, world!");
tagstop
```

then save and pass the filename as an argument to the tag compiler

`./tag helloworld.tag`

and run the generated out file!
```
./out
Hello, world!
```

Thanks for checking this out. It's probably not going anywhere and I have no real big plans for the future, but if you want to see potential syntax and the current grammar implemented, go visit the`docs/` folder in this repo.

### Roadmap, current and potential future features

- [x] `ret <int>;` Return from program with `int` exit code 
- [ ] `disp("string");` Print `string` to screen 
- [ ]  variable assignment and data types 
- [ ]  `if`, `else if`, `else`
- [ ] `for`, `for each`, and `while` loops 
- [ ]  functions
- [ ]  classes

#### Other cool stuff that I may get around to eventually

- [ ] AVR compiler to compile tag code for Arduino
- [ ] 6502 compiler to compile tag code for Ben Eater's homemade computer









