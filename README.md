# nautilus-shell
A robust, minimalist Unix shell written in C.

## Overview
A simple shell implementation that handles redirections, piping and environment variables.

* Nautilus handles simple commands (like `ls`, `cat` or `grep` via ``execvp()`` in a child process).
* You can use pipes (|) to chain commands together.
* '>' can be used to redirect output (e.g the output of `ls`) to a file specified.
* Nautilus automatically unwraps all environment variables ($USER, $HOME, etc.) and replaces them with their values.
* The `cd` command is handled by the ``chdir()`` function.

## Compiling from source
```bash
git clone https://github.com/tankz3508/nautilus-shell.git
cd nautilus-shell
cmake -B build -G Ninja
cmake --build build
./build/nautilus
```

This a very minimalist and simple implementation of a shell, however I used this as an opportunity to learn more about the C language and the Linux kernel.
