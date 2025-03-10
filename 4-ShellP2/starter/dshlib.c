#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
#include <errno.h>

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

int execute_command(char *cmd);
void parse_command(char *cmd, char **args);

int exec_local_cmd_loop() {
    char cmd_buff[ARG_MAX];

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, sizeof(cmd_buff), stdin) == NULL) {
            if (feof(stdin)) {
                printf("\nExiting shell.\n");
                return 0;
            } else {
                perror("Error reading input");
                continue;
            }
        }
 
        size_t len = strlen(cmd_buff);
        if (len > 0 && cmd_buff[len - 1] == '\n') {
            cmd_buff[len - 1] = '\0';
        }
 
        if (*cmd_buff == '\0') {
            continue;
        }
 
        if (execute_command(cmd_buff) == -1) {
            fprintf(stderr, "Command execution failed\n");
        }
    }
    return 0;
}

int execute_command(char *cmd) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return -1;
    } else if (pid == 0) {
        char *args[ARG_MAX];
        parse_command(cmd, args);
        if (execvp(args[0], args) == -1) {
            perror("Execution error");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
    return 0;
}
 
void parse_command(char *cmd, char **args) {
    int i = 0;
    char *token = strtok(cmd, " ");
    while (token != NULL && i < ARG_MAX - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

 int is_builtin_command(char *cmd) {
    return (strcmp(cmd, "exit") == 0 || strcmp(cmd, "cd") == 0);
}
 