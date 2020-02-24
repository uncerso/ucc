# ucc
Universal Compiler Collection

# Installation
1) install <code>g++</code> and <code>make</code>
2) run <code>make</code>

# Usage
To build and run a program from sources:

<code> ./run [-v] source1 source2 ... [--args arg1 arg2 ...]</code>

# Tests
<code> make build_and_run_tests</code>

# Compilers
If you want to compile a file in some language, you have to additionally install its compiler from the following table or run <code> install_dependencies_for_fedora.sh </code>

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
