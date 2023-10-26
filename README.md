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

## Building and Running
The provided Makefile facilitates building the main program, generating shared object files, and running tests.

### Compile the Main Program:
```bash
make main
```

### Generate Shared Object File:
```bash
make generate_so
```

### Run File Tests:
```bash
make run_file_tests
```

### Run Interactive Terminal:
```bash
make run_terminal
```

## Example Usage

### Script File:
```plaintext
# Load the library
use /path/to/library.so

# Call a function
call function_name

# Using multiple commands in line.
use /path/to/library.so;call function_name
```

### Interactive Mode:
```bash
gcc src/main.c -o bin/sotest.bin -ldl
gcc -fPIC -shared src/lib1.c -o bin/test.so
bin/sotest.bin
>>> use /path/to/library.so
>>> call function_name
>>> call \
... function_name
```

## Error Handling

Syntax and command execution errors are handled gracefully with interpreter warnings and termination respectively.

## Docker instance

### Setting Up Docker Development Environment
Docker is a tool designed to make it easier to create, deploy, and run applications by using containers. Containers allow a developer to package up an application with all parts of it including libraries and other dependencies and ship it all out as one package.

If it's running the software in others SO diferent to UNIX, Docker allow run and test the code in a lite GNU/Linux SO.

### Prerequisites:
- Docker installed on your machine. [Get Docker](https://docs.docker.com/get-docker/)

### Set the Dev enviroment:

* Open Docker dekstop.
* Go to the tab "Dev enviroments (beta)".
* Press the button "create +".
* Set in Git "Existing git repo" with value "https://github.com/JohanValero/test-cli.git" and IDE "Visual Studio Code" or your favorite IDE.
* Press button "continue".
* Wait the image build.
* When the image is build then press the button "continue".
* And press in "OPEN IN VSCODE" or your prefered IDE.
* In the IDE open the terminal in the Docker container instance.
* In the terminal type the commands:

```bash
make run_file_tests
```

```bash
make run_terminal
```