# C Template

[![gperftools](https://github.com/h3nnn4n/C_Template/actions/workflows/heap-check.yml/badge.svg)](https://github.com/h3nnn4n/c_template/actions/workflows/heap-check.yml)
[![valgrind](https://github.com/h3nnn4n/C_Template/actions/workflows/valgrind.yml/badge.svg)](https://github.com/h3nnn4n/C_Template/actions/workflows/valgrind.yml)
[![clang-format](https://github.com/h3nnn4n/C_Template/actions/workflows/clang-format-check.yml/badge.svg)](https://github.com/h3nnn4n/c_template/actions/workflows/clang-format-check.yml)
[![clang-tidy](https://github.com/h3nnn4n/C_Template/actions/workflows/clang-tidy.yml/badge.svg)](https://github.com/h3nnn4n/c_template/actions/workflows/clang-tidy.yml)
[![cppcheck](https://github.com/h3nnn4n/C_Template/actions/workflows/cppcheck.yml/badge.svg)](https://github.com/h3nnn4n/c_template/actions/workflows/cppcheck.yml)
[![cpplint](https://github.com/h3nnn4n/C_Template/actions/workflows/cpplint.yml/badge.svg)](https://github.com/h3nnn4n/c_template/actions/workflows/cpplint.yml)
[![Smoke Test](https://github.com/h3nnn4n/C_Template/actions/workflows/run.yml/badge.svg)](https://github.com/h3nnn4n/c_template/actions/workflows/run.yml)
[![Unit Tests](https://github.com/h3nnn4n/C_Template/actions/workflows/tests.yml/badge.svg)](https://github.com/h3nnn4n/c_template/actions/workflows/tests.yml)


Over time I have build a set of linters, checkers and other tools that I find
very helpful to use when working with pure C code. This repository contains a
basic setup of those tools on a very simple application.

It has the following set up:
1) clang-format (Code style)
2) clang-tidy (Linter)
3) cppcheck (Linter)
4) cpplint (Linter)
5) gproftools (Heap checker)
6) Valgrind (Heap checker)
7) Unity (C Unit Testing framework)

It also has a smoke test CI stage that simply runs the executable with a few
different args. This is mostly to check that it at least runs and exits
properly.

# Usage

1) Fork / clone this repo
2) Hack away

# License

All the contents of this repository are released under a MIT No Attribution
License, unless otherwise noted (such as the files under `deps/`). See
[LICENSE](LICENSE) for more details.
