#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


void shell_loop();
char *input_line();
char **input_args(char *line);
int execute(char **args);
int launch(char **args);

//builtin shell commands declarations
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

int shell_num_builtin();
int (*builtin_func[])(char **args);
char *builtin_str[];



int main(){
    shell_loop();    
}


void shell_loop(){
        int status = 0;
        char *line;
        char **args;

    do{
        status = 0;
        printf("> ");

        line = input_line();
        args = input_args(line);
        status = execute(args);

        free(args);
        free(line);                                                                         // free the memory allocated to buf in input_line
    }while(status == 1);
    
}


char* input_line(){
    
    char *buf = malloc(BUFSIZ);                                                                 // to store stdin
    size_t curr_buf_size = 0;                                                                   // to track number of chars in buf

    int n = getchar();                                                                          // get one char from stdin
    while((n != EOF) && (n != '\n')){                                                           // check if stdin char is EOF or new line char
        buf[curr_buf_size++] = n;                                                               // assign the char from stdin and then move curr_buf_size to so that we dont overwrite the char we just wrote
        n = getchar(); 

    }

    buf[curr_buf_size] = '\0';                                                                  // terminate the line with a string terminator char, so that we can use it to signal the end of string

    return buf;                                                                                 //return stdin line content
}

char **input_args(char *line){
    char **args = malloc(10 * sizeof(char *));                                                   //to store tokens from user input, basically arguments
    int argc = 0;                                                                               // track number of arguments passed by user

    char *token = strtok(line, " ");                                                            // store each work in the line input user inputted

    while(token != NULL && argc < 3){                                                           //loop to parse each word in line into args, max number or args is 3(inclusive of function arg)
        args[argc] = token;
        token = strtok(NULL, " ");
        argc++;
    }

    args[argc] = NULL;     

    return args;                                                                                // no to return the list of args
}

//function to execute programs
int execute(char **args){
    int i;

    if(args[0] == NULL){
        return 1;
    }
        for (i = 0; i < shell_num_builtin(); i++){
            if(strcmp(args[0], builtin_str[i]) == 0){
                return (*builtin_func[i])(args);
            }
        }

    return launch(args);
}

//function to launch programs
int launch(char **args){
    pid_t pid = fork();
    int status;

    if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror("myshell");
        }
        exit(EXIT_FAILURE);
    }else if (pid < 0){
        //will print error when forking
        perror("myShell");
    }else{
        do{
            waitpid(pid, &status,WUNTRACED);
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

//list of builitin functions
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

//returns  number of builtin fucntions
int shell_num_builtin(){
    return sizeof(builtin_str) / sizeof(char *);
}


int (*builtin_func[])(char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit
};

int shell_help(char **args){
    int i;
    printf("This is my simple shell\n");
    printf("type a program name and argument and then press enter\n");
    printf("this shell has these builtin functions\n");

    for(i = 0; i < shell_num_builtin(); i++){
        printf("  %s\n", builtin_str[i]);
    }

    printf("use the man command for information on other programs\n");
    return 1;
}

int shell_cd(char **args){
    if(chdir(args[1]) != 0){
        perror("failed to change to dir");
    }
    return 1;
}

int shell_exit(char **args){
    exit(1);

    return 1;
}
