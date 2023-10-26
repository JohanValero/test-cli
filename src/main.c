#include <dlfcn.h>      // Library for load shared library (.so files).
#include <stdio.h>      // Library for printf and other common methods.
#include <string.h>     // Library for string processing.
#include <stdlib.h>     // Standart library with program constant.

// Constants basic in the compilation.
#define END_OF_STRING    '\0'
#define JUMP_OF_LINE     '\n'
#define END_OF_STATEMENT ';'
#define COMMENT_CHAR     '#'
#define JUMP_OF_COMMAND  '\\'
#define BUFFER_SIZE      1024

// Comands definitions for use in the code.
#define COMAND_USE     "use"
#define COMAND_CALL    "call"
#define COMAND_COMMENT "#"

// Constants for identify commands.
#define CONST_USE    1
#define CONST_CALL   2

// Basic definitions for logic operations.
#define and &&
#define or  ||
#define not !

// Boolean type Boolean definition and true/false value. 
#define bool  unsigned char
#define true  1
#define false 0

// Methods prototype.
void process_file(char *);
void str_replace(char *, char, char);
void handle_line(char *, char *, void **);
bool get_command(char *, FILE *);
void get_command_cli(char *);
void terminal_loop();

// Main method.
int main(int argc, char * argv []) {
    printf("** Program C comand line interface started.\n");

    // If there's not arguments then use the terminal mode.
    if(argc == 1){
        terminal_loop();
    }

    // If exists one argument, then process the file.
    if(argc == 2){
        process_file(argv[1]);
    }

    printf("** Program C comand line interface finished.\n");
    return EXIT_SUCCESS;
}

// Methods.

/**
 * Method name: str_replace
 * Method for executing a terminal instance.
 * This method initiates an interactive mode where the user can enter commands.
 * The loop continues until the user types 'quit'.
 * It utilizes other helper methods like get_command_cli and handle_line to process the user input.
 * 
 * Parameters:
 *  - No parameters.
*/
void terminal_loop(){
    printf(">>> Actived intrepeter mode.\n");
    printf(">>> for exit of terminal type 'quit'.\n");
    
    char buffer[BUFFER_SIZE];   // Buffer to hold the user input.
    char so_path[BUFFER_SIZE];  // Buffer to hold the shared object path.
    bool flag = true;           // Control flag for the do-while loop.
    void * so_lib = NULL;       // Pointer to hold the shared object library handle.

    do {
        // Get user input.
        get_command_cli(buffer);

        // Check if the user wants to quit.
        if(strcmp(buffer, "quit") == 0){
            break;
        }

        // Get the length of the user input.
        size_t len = strlen(buffer);

        // Check if the input is empty and the flag is true for break the continue to the next command.
        if(len == 0 and flag){
            continue;
        }

        // Handle the user input.
        handle_line(buffer, so_path, &so_lib);
    } while(flag);
}

// Method for get an command in the CLI.
void get_command_cli(char * buffer){
    printf(">>> ");
    unsigned short index = 0;
    bool flag = true;
    char inside_buffer[BUFFER_SIZE];
    char * ptr = buffer;

    do {
        fgets(inside_buffer, BUFFER_SIZE, stdin);
        str_replace(inside_buffer, COMMENT_CHAR, END_OF_STRING);
        str_replace(inside_buffer, JUMP_OF_LINE, END_OF_STRING);

        index = strlen(inside_buffer);
        if(index > 0 and inside_buffer[index - 1] == JUMP_OF_COMMAND){
            strcpy(ptr, inside_buffer);
            ptr = buffer + index - 1;
            printf("... ");
            continue;
        }
        strcpy(ptr, inside_buffer);
        flag = false;
    } while(flag);
}

/**
 * str_replace
 * Purpose: Replaces all occurrences of i_source in i_string with i_destiny.
 * 
 * Parameters:
 *  - i_string: The string to modify.
 *  - i_source: The character to be replaced.
 *  - i_destiny: The replacement character.
*/
void str_replace(char * i_string, char i_source, char i_destiny) {
    short i = 0;
    while(i_string[i] != END_OF_STRING){
        if(i_string[i] == i_source){
            i_string[i] = i_destiny;
            if(i_destiny == END_OF_STRING){
                break;
            }
        }
        i++;
    }
}

/**
 * process_file
 * Purpose: Reads a script file and executes the commands within.
 * 
 * Parameters:
 *  - i_file_path: Path to the script file to be processed.
 * Description:
 *      Reads each line from the script file.
 *      Interprets the line based on the command ("use" or "call").
 *      For the "use" command, attempts to load a shared library.
 *      For the "call" command, attempts to call a function from the loaded library.
 *      Handles and reports errors like file not found, function not found, or syntax errors.
*/
void process_file(char * i_file_path){
    char buffer[BUFFER_SIZE];
    char so_path[BUFFER_SIZE];
    void * so_lib = NULL;
    FILE * file;
    bool flag = false;

    file = fopen(i_file_path, "r");
    if (not file) {
        perror("Failed to open script file.\n");
        exit(EXIT_FAILURE);
    }

    do {
        flag = get_command(buffer, file);
        size_t len = strlen(buffer);
        printf("%s\n", buffer);

        if(len == 0 and flag){
            continue;
        }

        handle_line(buffer, so_path, &so_lib);
    } while(flag);

    printf("File process finished.\n");
    if(so_lib) {
        dlclose(so_lib);
    }
    fclose(file);
}

bool get_command(char * buffer, FILE * file) {
    size_t index = 0;
    char ch;
    bool isInAComentary = false;
    bool isInJumpOfLine = false;
    
    while((ch = fgetc(file)) != EOF){
        if(isInJumpOfLine and (ch != JUMP_OF_LINE)){
            buffer[index] = JUMP_OF_COMMAND;
            buffer[index + 1] = END_OF_STRING;
            return true;

            printf(" * SyntaxError - unterminated string literal. Command: %s\n", buffer);
            index = 0;
            isInJumpOfLine = false;
            isInAComentary = false;
        }

        if(ch == JUMP_OF_LINE){
            buffer[index] = END_OF_STRING;
            if(not isInJumpOfLine and index > 0){
                return true;
                printf(" 1 %s\n", buffer);
            }

            if(not isInJumpOfLine){
                index = 0;
            }
            isInAComentary = false;
            isInJumpOfLine = false;
        } else if(ch == END_OF_STATEMENT and not isInAComentary){
            buffer[index] = END_OF_STRING;
            
            if(index > 0){
                return true;
                printf(" 2 %s\n", buffer);
            }

            index = 0;
            isInAComentary = false;
            isInJumpOfLine = false;
        } else if(ch == COMMENT_CHAR and not isInAComentary) {
            buffer[index] = END_OF_STRING;
            isInAComentary = true;
        } else if(ch == JUMP_OF_COMMAND and not isInAComentary){
            isInJumpOfLine = true;
        } else if(not isInAComentary){
            buffer[index] = ch;
            index++;
        }
    }
    buffer[index] = END_OF_STRING;
    return false;
    printf("Process finished with character [%c] and size [%d] and string [%s].\n", ch, index, buffer);
}

void handle_line(char * buffer, char * so_path, void ** so_lib){
    char * ptr_split;
    short comand_const = 0;

    ptr_split = strtok(buffer, " ");
    if(ptr_split == NULL){
        printf("--> Syntax error. Comand ignored.\n");
        return;
    }

    if(strcmp(ptr_split, COMAND_USE) == 0){
        comand_const = CONST_USE;
    } else if(strcmp(ptr_split, COMAND_CALL) == 0){
        comand_const = CONST_CALL;
    } else {
        printf("--> Syntax error. Comand ignored.\n");
        return;
    }

    ptr_split = strtok(NULL, " ");

    if(ptr_split == NULL){
        printf("--> Syntax error. Comand ignored.\n");
        return;
    }

    if(comand_const == CONST_USE){
        strcpy(so_path, ptr_split);
        *so_lib = NULL;

        FILE * temp_file = fopen(so_path, "r");
        if(not temp_file){
            printf("--> The library [%s] was not found.\n", so_path);
            return;
        }
        fclose(temp_file);

        if(*so_lib) {
            dlclose(*so_lib);
        }
        *so_lib = dlopen(so_path, RTLD_LAZY);
        if(not *so_lib) {
            printf("--> The file [%s] is not a shared library.\n", so_path);
        }

        return;
    }
    
    if(comand_const == CONST_CALL){
        if(not *so_lib) {
            printf("--> The library was not previously loaded.\n");
            return;
        }
        void (*test_func)() = dlsym(*so_lib, ptr_split);

        if(test_func == NULL) {
            printf("--> The function [%s] was not found.\n", ptr_split);
            return;
        }
        test_func();
    }
}