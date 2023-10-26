# Shared Functions Libraries Testing Interpreter

A console application developed in C language for GNU/Linux (x86 processor architecture) adhering to the API POSIX.1-2001 standards. This interpreter is designed to test shared function libraries by executing script files or interactive terminal inputs.

## Features

- Script file execution with one line representing one command.
- Interactive mode for entering commands directly from the terminal.
- Supports loading shared libraries and calling functions within those libraries.
- Error handling for file not found, file not a shared library, function not found, and library not loaded scenarios.
- Syntax errors trigger interpreter warnings without halting script execution.
- Command execution errors terminate script execution.
- Supports comments (`#`, `;`) and line breaks (`\`).

## Scripting Language Syntax

### Loading the Library:
```plaintext
use <so_name>
`<so_name>` - the path to the .so file.
```

### Library Function Call:
```plaintext
call <func_name>
`<func_name>` - the name of the function.
```