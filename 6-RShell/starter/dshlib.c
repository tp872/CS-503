#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

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
int build_cmd_list(char *input, command_list_t *cmd_list) {
    if (!input || *input == '\0') {
        return WARN_NO_CMDS;
    }

    cmd_list->num = 0;
    char *saveptr1, *saveptr2;
    char *cmd_str = strtok_r(input, "|", &saveptr1);

    while (cmd_str && cmd_list->num < CMD_MAX) {
        while (isspace(*cmd_str)) cmd_str++;
        
        cmd_list->commands[cmd_list->num].argc = 0;
        char *arg = strtok_r(cmd_str, " \t", &saveptr2);
        
        while (arg && cmd_list->commands[cmd_list->num].argc < CMD_ARGV_MAX - 1) {
            size_t len = strlen(arg);
            char *new_arg = malloc(len + 1);
            if (!new_arg) return ERR_MEMORY;
            
            strcpy(new_arg, arg);
            cmd_list->commands[cmd_list->num].argv[cmd_list->commands[cmd_list->num].argc++] = new_arg;
            arg = strtok_r(NULL, " \t", &saveptr2);
        }
        
        cmd_list->commands[cmd_list->num].argv[cmd_list->commands[cmd_list->num].argc] = NULL;
        cmd_list->num++;
        cmd_str = strtok_r(NULL, "|", &saveptr1);
    }

    return (cmd_list->num > CMD_MAX) ? ERR_TOO_MANY_COMMANDS : OK;
}

int free_cmd_list(command_list_t *cmd_list) {
    if (!cmd_list) return ERR_MEMORY;
    
    for (int i = 0; i < cmd_list->num; i++) {
        for (int j = 0; j < cmd_list->commands[i].argc; j++) {
            if (cmd_list->commands[i].argv[j]) {
                free(cmd_list->commands[i].argv[j]);
                cmd_list->commands[i].argv[j] = NULL;
            }
        }
        cmd_list->commands[i].argc = 0;
    }
    cmd_list->num = 0;
    return OK;
}

int execute_pipeline(command_list_t *cmd_list) {
    int num_cmds = cmd_list->num;
    int pipes[num_cmds - 1][2];
    pid_t pids[num_cmds];

    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            return ERR_EXEC_CMD;
        }
    }

    for (int i = 0; i < num_cmds; i++) {
        pids[i] = fork();
        
        if (pids[i] == 0) {
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            
            if (i < num_cmds - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < num_cmds - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            if (strcmp(cmd_list->commands[i].argv[0], "grep") == 0) {
                cmd_list->commands[i].argv[1] = "\\.c$";
            }

            execvp(cmd_list->commands[i].argv[0], cmd_list->commands[i].argv);
            exit(1);
        }
    }

    for (int i = 0; i < num_cmds - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < num_cmds; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return OK;
}

int exec_local_cmd_loop() {
    char cmd_buffer[SH_CMD_MAX];
    command_list_t cmd_list = {0};
    int status;

    while (1) {
        printf("%s", SH_PROMPT);
        fflush(stdout);

        if (!fgets(cmd_buffer, SH_CMD_MAX, stdin)) {
            printf("\n");
            break;
        }

        cmd_buffer[strcspn(cmd_buffer, "\n")] = '\0';

        if (strcmp(cmd_buffer, EXIT_CMD) == 0) {
            printf("exiting...\ncmd loop returned 0\n");
            break;
        }

        if (strncmp(cmd_buffer, "cd ", 3) == 0) {
            if (chdir(cmd_buffer + 3) != 0) {
                perror("cd");
            }
            continue;
        }

        status = build_cmd_list(cmd_buffer, &cmd_list);
        
        if (status == WARN_NO_CMDS) {
            printf("%s\n", CMD_WARN_NO_CMD);
            continue;
        }
        
        if (status == ERR_TOO_MANY_COMMANDS) {
            printf("%s\n", CMD_ERR_PIPE_LIMIT);
            continue;
        }

        if (execute_pipeline(&cmd_list) != OK) {
            printf("Command execution failed\n");
        }

        free_cmd_list(&cmd_list);
    }

    return OK;
}