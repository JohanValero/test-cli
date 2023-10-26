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

/**
 * Method name: get_command_cli
 * Method for getting a command in the CLI (Command Line Interface).
 * This method prompts the user to enter a command, processes the command to handle special characters,
 * and stores the command in the provided buffer.
 * It utilizes a do-while loop to ensure the entire command is captured, especially in case of multi-line commands.
 * It also uses the str_replace method to handle special characters within the command.
 * 
 * Parameters:
 *  - buffer: A pointer to a char array where the command will be stored.
 */
void get_command_cli(char * buffer){
    printf(">>> ");
    unsigned short index = 0;           // Index to keep track of the position within the buffer.
    char inside_buffer[BUFFER_SIZE];    // Temporary buffer to hold the current line of input.
    char * ptr = buffer;                // Pointer to keep track of the position within the main buffer.

    do {
        fgets(inside_buffer, BUFFER_SIZE, stdin);                   // Get the current line of input from the user.
        str_replace(inside_buffer, COMMENT_CHAR, END_OF_STRING);    // Remove comment characters from the input.
        str_replace(inside_buffer, JUMP_OF_LINE, END_OF_STRING);    // Remove new line characters from the input.
        index = strlen(inside_buffer);                              // Get the length of the current line of input.

        // Check if the current line ends with a command continuation character.
        if(index > 0 and inside_buffer[index - 1] == JUMP_OF_COMMAND){
            printf("... ");
            strcpy(ptr, inside_buffer);     // Copy the current line to the main buffer.
            ptr = buffer + index - 1;       // Update the pointer to point to the position where the next line should be added.
            continue;                       // Continue to the next iteration to get the next line of the command.
        }
        strcpy(ptr, inside_buffer);         // Copy the current line to the main buffer.
        break;
    } while(true);
}

/**
 * Method name: str_replace
 * Method for replacing occurrences of a character with another character in a string.
 * This method iterates through the given string, and for each occurrence of the source character,
 * it replaces it with the destiny character. If the destiny character is the null terminator,
 * it breaks out of the loop early.
 * 
 * Parameters:
 *  - i_string: A pointer to the string to be modified.
 *  - i_source: The character to be replaced.
 *  - i_destiny: The replacement character.
 */
void str_replace(char * i_string, char i_source, char i_destiny) {
    unsigned char i = 0;
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
 * Method name: process_file
 * Method for processing a script file.
 * This method opens the specified script file, reads and processes commands from it line by line.
 * It utilizes other helper methods like get_command and handle_line to process each command.
 * It also manages a shared object library, which can be loaded and utilized based on the commands in the script.
 * 
 * Parameters:
 *  - i_file_path: A pointer to a string containing the path to the script file to be processed.
 */
void process_file(char * i_file_path){
    char buffer [BUFFER_SIZE];      // Buffer to hold the commands read from the file.
    char so_path[BUFFER_SIZE];      // Buffer to hold the path of the shared object library.
    void * so_lib = NULL;           // Pointer to hold the shared object library handle.
    FILE * file;                    // File pointer for the script file.
    bool flag = false;              // Control flag for the do-while loop.

    file = fopen(i_file_path, "r"); // Open the script file for reading.
    if (not file) {
        perror("Failed to open script file.\n");
        exit(EXIT_FAILURE);
    }

    do {
        flag = get_command(buffer, file);   // Get the next command from the file.
        size_t len = strlen(buffer);
        printf("%s\n", buffer);

        // Check if the command is empty.
        // For skip to the next iteration to get the next command
        if(len == 0){
            continue;
        }

        // Handle the command.
        handle_line(buffer, so_path, &so_lib);
    } while(flag);

    printf("File process finished.\n");

    // Free resources.
    if(so_lib) {
        dlclose(so_lib);
    }
    fclose(file);
}

/**
 * Method name: get_command
 * Method for extracting a command from a file.
 * This method reads characters from a file one by one until it forms a complete command,
 * based on the specified terminating characters. It handles special situations like
 * comments and command continuation over multiple lines.
 * 
 * Parameters:
 *  - buffer: A pointer to a char array where the command will be stored.
 *  - file: A pointer to the file from which commands will be read.
 * 
 * Returns:
 *  - A boolean value indicating whether a command was successfully read (true) or if the end of file was reached (false).
 */
bool get_command(char * buffer, FILE * file) {
    size_t index = 0;               // Index to keep track of the position within the buffer.
    char ch;                        // Character variable to hold the current character being read from the file.
    bool isInAComentary = false;    // Flag to indicate whether the current position is within a comment.
    bool isInJumpOfLine = false;    // Flag to indicate whether the current position is at the continuation of a command over multiple lines.
    
    // Continue reading characters from the file until the end of file is reached.
    while((ch = fgetc(file)) != EOF){
        // Check if in a multi-line command and the current character is not a new line character.
        if(isInJumpOfLine and (ch != JUMP_OF_LINE)){
            buffer[index] = JUMP_OF_COMMAND;
            buffer[index + 1] = END_OF_STRING;
            return true;

            // Debug code.
            printf(" * SyntaxError - unterminated string literal. Command: %s\n", buffer);
            index = 0;
            isInJumpOfLine = false;
            isInAComentary = false;
        }

        // If it's a new line, and verify if it's adding a multi-line command. 
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
        }
        // If it's adding multiples comand in one line. And not it's a comment.
        else if(ch == END_OF_STATEMENT and not isInAComentary){
            buffer[index] = END_OF_STRING;
            
            if(index > 0){
                return true;
                printf(" 2 %s\n", buffer);
            }

            index = 0;
            isInAComentary = false;
            isInJumpOfLine = false;
        }
        // If start a comment.
        else if(ch == COMMENT_CHAR and not isInAComentary) {
            buffer[index] = END_OF_STRING;
            isInAComentary = true;
        }
        // If it's adding an multi-line command.
        else if(ch == JUMP_OF_COMMAND and not isInAComentary){
            isInJumpOfLine = true;
        }
        // Add the valid character to the command.
        else if(not isInAComentary){
            buffer[index] = ch;
            index++;
        }
    }
    buffer[index] = END_OF_STRING;
    return false;
    printf("Process finished with character [%c] and size [%d] and string [%s].\n", ch, index, buffer);
}

/**
 * Method name: handle_line
 * Method for handling a single line of command.
 * This method parses a command from a string, determines the type of command (either "use" or "call"),
 * and acts accordingly. If it's a "use" command, it attempts to load a specified shared library.
 * If it's a "call" command, it attempts to invoke a specified function from the currently loaded library.
 * 
 * Parameters:
 *  - buffer: A pointer to a char array containing the command to be handled.
 *  - so_path: A pointer to a char array where the path to the shared object library will be stored.
 *  - so_lib: A pointer to a pointer where the handle to the shared object library will be stored.
 */
void handle_line(char * buffer, char * so_path, void ** so_lib){
    char * ptr_split;           // Pointer for tokenization of the command string.
    short comand_const = 0;     // Variable to hold the constant representing the command type.

    // Tokenize the command string to get the command type.
    ptr_split = strtok(buffer, " ");
    if(ptr_split == NULL){
        printf("--> Syntax error. Comand ignored.\n");
        return;
    }

    // Determine the command type by comparing the first token to known command strings.
    if(strcmp(ptr_split, COMAND_USE) == 0){
        comand_const = CONST_USE;
    } else if(strcmp(ptr_split, COMAND_CALL) == 0){
        comand_const = CONST_CALL;
    } else {
        printf("--> Syntax error. Comand ignored.\n");
        return;
    }

    // Tokenize again to get the argument for the command.
    ptr_split = strtok(NULL, " ");

    if(ptr_split == NULL){
        printf("--> Syntax error. Comand ignored.\n");
        return;
    }

    // Load the Shared library.
    if(comand_const == CONST_USE){
        strcpy(so_path, ptr_split);     // Copy the library path from the command argument.
        
        // Reset the shared library handle.
        if(*so_lib) {
            dlclose(*so_lib);
            *so_lib = NULL;
        }

        // Check for file opening error.
        FILE * temp_file = fopen(so_path, "r");
        if(not temp_file){
            printf("--> The .so file [%s] was not found.\n", so_path);
            return;
        }
        fclose(temp_file);

        // Attempt to load the shared library.
        *so_lib = dlopen(so_path, RTLD_LAZY);
        if(not *so_lib) {
            printf("--> The file [%s] is not a shared library.\n", so_path);
        }

        return;
    }
    
    // Call a function from the loaded library.
    if(comand_const == CONST_CALL){
        // Check if a library has been loaded.
        if(not *so_lib) {
            printf("--> The library was not previously loaded.\n");
            return;
        }

        // Attempt to get the function pointer from the library.
        void (*test_func)() = dlsym(*so_lib, ptr_split);

        // Check for function retrieval error.
        if(test_func == NULL) {
            printf("--> The function [%s] was not found.\n", ptr_split);
            return;
        }

        // Call the command function.
        test_func();
    }
}