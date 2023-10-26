#include <dlfcn.h>      // Library for load shared library (.so files).
#include <stdio.h>      // Library for printf and other common methods.
#include <string.h>     // Library for string processing.
#include <stdlib.h>     // Standart library with program constant.

// Constants basic in the compilation.
#define END_OF_STRING '\0'
#define JUMP_OF_LINE  '\n'
#define BUFFER_SIZE   1024

#define COMAND_USE     "use"
#define COMAND_CALL    "call"
#define COMAND_COMMENT "#"

#define CONST_USE    1
#define CONST_CALL   2

// Methods prototype.
void process_file(char *);
void str_replace(char *, char, char);

// Main method.
int main(int argc, char * argv []) {
    if(argc == 1){
        printf("Actived intrepeter mode.\n");
    }

    if(argc == 2){
        process_file(argv[1]);
    }

    return EXIT_SUCCESS;
}

// Methods.

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
    char * ptr_split;
    FILE * file;

    short comand_const = 0;

    printf("File to read: %s\n", i_file_path);
    file = fopen(i_file_path, "r");
    if (!file) {
        perror("Failed to open script file.\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t len = strlen(buffer);
        if(len == 0) continue;
        buffer[len-1] = END_OF_STRING;

        str_replace(buffer, '#', END_OF_STRING);
        len = strlen(buffer);

        if(len == 0){
            continue;
        }
        printf("> %s\n", buffer);

        ptr_split = strtok(buffer, " ");
        if(ptr_split == NULL){
            printf("- Syntax error. Line ignored.\n");
        }

        if(strcmp(ptr_split, COMAND_USE) == 0){
            comand_const = CONST_USE;
        } else if(strcmp(ptr_split, COMAND_CALL) == 0){
            comand_const = CONST_CALL;
        } else {
            printf("- Syntax error. Line ignored.\n");
            continue;
        }

        ptr_split = strtok(NULL, "");

        if(ptr_split == NULL){
            printf("- Syntax error. Line ignored.\n");
            continue;
        }

        if(comand_const == CONST_USE){
            strcpy(so_path, ptr_split);
            so_lib = NULL;

            FILE * temp_file = fopen(so_path, "r");
            if(!temp_file){
                printf("--> The file [%s] was not found.\n", so_path);
                continue;
            }
            fclose(temp_file);

            if(so_lib) {
                dlclose(so_lib);
            }
            so_lib = dlopen(so_path, RTLD_LAZY);
            if(!so_lib) {
                printf("--> The file [%s] is not a shared library.\n", so_path);
            }
        } else if(comand_const == CONST_CALL){
            if(!so_lib) {
                printf("--> The library was not previously loaded.\n");
                continue;
            }
            void (*test_func)() = dlsym(so_lib, ptr_split);

            if(test_func == NULL) {
                printf("--> The function [%s] was not found.\n", ptr_split);
                continue;
            }
            test_func();
        }
    }
    printf("File process finished.\n");
    fclose(file);
}
