# ucc
Universal Compiler Collection

# How to compile
1) install <code>g++</code> and <code>make</code>
2) run <code>make</code>

# How to use
To build and run a program from sources:
<code> ./run [-v] source1 source2 ... [--args arg1 arg2 ...]</code>

# Compilers
If you want to compile a file in some language, you have to additionally install its compiler from the following table or run install_dependencies_for_fedora.sh

| Language | Compiler |
| -------- | -------- |
| C        | gcc      |
| C++      | g++      |
| D        | gdc      |
| Haskell  | ghc      |
| Pascal   | fpc      |
| Fortran  | gfortran |
| Ada      | gnat     |
| OCaml    | ocamlc   |
| Lisp     | clisp    |

For interpreted languages, such as <code>bash, python, csh, perl</code>, you should have an appropriate interpreter.
