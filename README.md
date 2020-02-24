# ucc
Universal Compiler Collection

# How to compile
1) install <code>g++</code> and <code>make</code>
2) run <code>make</code>

# How to use
<code> ./ucc path_to_the_file_to_be_compiled </code>

The result of the compilation will be placed in the current work directory and named <code>out</code>.

# Compilers
If you want to compile a file in some language, you have to additionally install its compiler from the following table

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

For interpreted languages, such as <code>bash, python, csh, perl</code>, you should have an appropriate interpreter.
