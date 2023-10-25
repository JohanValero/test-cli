#include <dlfcn.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define END_OF_STRING '\0'
#define JUMP_OF_LINE  '\n'
#define COMAND_USE    "use"
#define COMAND_CALL   "call"
#define BUFFER_SIZE   1024

void process_file(char * i_file_path);

int main(int argc, char * argv []) {
    if(argc == 1){
        printf("Actived intrepeter mode.\n");
    }

    if(argc == 2){
        process_file(argv[1]);
    }
    return 0;
}

void process_file(char * i_file_path){
    FILE * file;
    char buffer[BUFFER_SIZE];
    char line  [BUFFER_SIZE];

    printf("Text file passed: %s\n", i_file_path);

    file = fopen(i_file_path, "r");
    if (!file) {
        perror("Failed to open script file.\n");
        exit(-1);
    }

    printf("Starting to read lines:\n");
    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t len = strlen(buffer);

        if (len > 0 && buffer[len-1] == JUMP_OF_LINE) {
            buffer[len-1] = END_OF_STRING;
        }

        strcpy(line, buffer);
        strtok(buffer, " ");

        if(buffer == NULL){
            printf("Comand empty.");
        }

        if(strcmp(buffer, COMAND_USE) == 0){
            printf("COMAND USE  > ");
        } else if(strcmp(buffer, COMAND_CALL) == 0){
            printf("COMAND CALL > ");
        }
        printf("Line: %20s > ", line);

        printf("Token: %s\n", buffer);
    }

    fclose(file);
}