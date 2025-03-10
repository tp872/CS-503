#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
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
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */
command_list_t *parse_commands(char *cmd_line);

void handle_exit() {
    exit(0);
}

void handle_dragon() {
    print_dragon();
}

void handle_command(char *input) {
    if (!input || *input == '\0') {
        printf(CMD_WARN_NO_CMD);
        return;
    }

    command_list_t *clist = parse_commands(input); // Explicit declaration prevents implicit warning
    if (!clist) {
        printf(CMD_WARN_NO_CMD);
        return;
    }

    printf("PARSED COMMAND LINE - TOTAL COMMANDS %d\n", clist->num);
    for (int i = 0; i < clist->num; i++) {
        printf("<%d> %s %s\n", i + 1, clist->commands[i].exe, clist->commands[i].args);
    }

    free(clist);
}

int main() {
    char *buffer = NULL;
    size_t bufsize = 0;

    while (1) {
        printf("%s", SH_PROMPT);

        ssize_t len = getline(&buffer, &bufsize, stdin);
        if (len == -1) {
            printf("\n");
            break;
        }

        // Trim trailing newline
        if (buffer[len - 1] == '\n') buffer[len - 1] = '\0';

        // Function pointer table for special commands
        struct {
            const char *cmd;
            void (*handler)();
        } commands[] = {
            {EXIT_CMD, handle_exit},
            {"/dragon", handle_dragon},
            {NULL, NULL}
        };

        int handled = 0;
        for (int i = 0; commands[i].cmd; i++) {
            if (strcmp(buffer, commands[i].cmd) == 0) {
                commands[i].handler();
                handled = 1;
                break;
            }
        }

        if (!handled) {
            handle_command(buffer);
        }
    }

    free(buffer);
    return 0;
}