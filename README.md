# JackCompiler (C++)

This is a C++ implementation of the Jack language compiler from projects 10-11 of nand2tetris.

The compiler takes a single `.jack` file or a directory of `.jack` files as a command-line argument and compiles each file into a `.vm` file.

## Modules

CompilationEngine: Processes tokens and determines compilation routines  
CompilerResources: Enums and tokens for program elements  
JackCompiler: Drives the compilation process  
JackTokenizer: Processes and tokenizes file input  
SymbolTable: Tracks symbol and variable names used in file  
VMWriter: Writes VM commands to output  
main: Program entry point  
utils: Helper functions for string and command-line argument processing

## Building the project

Run the following from the terminal:

```zsh
git clone https://github.com/midorigd/JackCompilerCpp
cd JackCompilerCpp
mkdir build
cd build
cmake ..
make
```

## Running the project

Run the following from the project directory:

```zsh
bin/JackCompiler <dirname OR filename.vm> [-d]
```

### Flags

`-d`: Enables symbol table debug file

## Notes

My Python implementation of this project: [JackCompiler (Python)](https://github.com/midorigd/JackCompilerPython)

### Stats

1786 lines (1449 loc)

## References

[nand2tetris](https://www.nand2tetris.org/course)

