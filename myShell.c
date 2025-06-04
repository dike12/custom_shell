#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* input_line(int *status){
    
    char *buf = malloc(BUFSIZ);                                                                 // to store stdin
    size_t curr_buf_size = 0;                                                                   // to track number of chars in buf

    int n = getchar();                                                                          // get one char from stdin
    while((n != EOF) && (n != '\n')){                                                           // check if stdin char is EOF or new line char
        buf[curr_buf_size++] = n;                                                               // assign the char from stdin and then move curr_buf_size to so that we dont overwrite the char we just wrote
        n = getchar(); 

    }
    buf[curr_buf_size] = '\0';                                                                  // terminate the line with a string terminator char, so that we can use it to signal the end of string

    *status = 1;
    return buf;                                                                                 //return stdin line content
}

char **input_args(char *line){
    char **args = malloc(3 * sizeof(char *));                                                   //to store tokens from user input, basically arguments
    int argc = 0;                                                                               // track number of arguments passed by user

    char *token = strtok(line, " ");                                                            // store each work in the line input user inputted

    while(token != NULL && argc < 3){                                                           //loop to parse each word in line into args, max number or args is 3(inclusive of function arg)
        args[argc] = token;
        token = strtok(NULL, " ");
        argc++;
    }

    args[3] = NULL;     

    return args;                                                                                // no to return the list of args
}

void shell_loop(){
        int status = 0;
        char *line;
        char **args;

    do{
        status = 0;
        printf("> ");

        line = input_line(&status);
        args = input_args(line);

        
        free(line);                                                                         // free the memory allocated to buf in input_line
        free(args);                                                                         // free the memory allocated to args in input_args
    }while(status == 1);
    
}

int main(int argc, int *argv[]){
    shell_loop();
    /*
    char *line = "hello there";

    char *first = strtok(line, " ");
    char *second = strtok(line, " ");

    printf("%s\n", first);
    printf("%s\n", second);
    */
}