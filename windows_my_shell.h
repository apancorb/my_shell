#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <io.h>
#include <process.h>
#include <sys/types.h> 
#include <windows.h>


#define VALID_LINE 1024


int process(char *line, FILE *input) {
    int words;

    if ((words = count_num_cmd(line)) != 0) {
        execute(line, words, input);
    } else { /* we have an empty line so it gets skipped */
        return FALSE;
    }
}

/* returns the number of words or commands in a given line */
int count_num_cmd(char *line) {
    int words = 0, i = 0, j = 0;

    if(isspace(line[0]) == 0) {
        words++;
    }

    /* count the number of words in a given line */
    while (line[i] != '\0') {
        if (isspace(line[i]) != 0 && isspace(line[i + 1]) == 0 && line[i + 1] != '\0') {
            words++;
        }
        i += 1;
    }
    
    return words;
}

void execute(char *line, int words, FILE *input) {
    int is_success;
    char first_str[VALID_LINE] = {0}, second_str[VALID_LINE] = {0};
    /* 
        There are two type of commands:
            1. Shell (cd, quit, sql, and any other command you want the shell to do)
            2. Non-Shell
    */
    sscanf(line, " %s %s", first_str, second_str);
    if ((strcmp(first_str, "quit") == 0 || strcmp(first_str, "exit") == 0)) {
        execute_quit();
    } else if (strcmp(first_str, "cd") == 0) {
        if ((is_success = execute_cd(second_str, words)) == FALSE) {
            fprintf(stdout, "There was an error changing directory. Run the <help> command for more information.\n");
        } 
    } else {
        execute_non_shell(line); 
    }
}

void execute_quit() {
    exit(EXIT_SUCCESS);
}

int execute_cd(char *second_str, int words) { 
    /* if we need to go to the home dir */
    if (words == 1) {
        char *loc = getenv("HOMEPATH");
        int is_chdir_success = chdir(loc);
        if (is_chdir_success == -1) {
            perror(loc);
            return FALSE; /* since the command was unsuccesful */
        } else {
            return TRUE;
        }
    } else if (words == 2) { /* else we got to the specified dir using the second argumnet of the command */
        int is_chdir_success = chdir(second_str);
        if (is_chdir_success == -1) {
            perror(second_str);
            return FALSE; /* since the command was unsuccesful */
        } else {
            return TRUE;
        }
    } else { /* Inavalid Command */
        fprintf(stderr, "cd error");
        fprintf(stdout, "Invalid Command, too many arguments for <cd>. Run the <help> command for more information.\n");
        return FALSE;
    }
}

void execute_non_shell(char *line) {
    char exe[VALID_LINE + 12] = "cmd.exe /c ";

    PROCESS_INFORMATION pi;
    STARTUPINFO si;
 
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb=sizeof(STARTUPINFO);
     
    /*
    si.cbReserved2 = 0;
    si.dwFillAttribute = 0;
    si.dwFlags = 0;
    si.dwX = 0;
    si.dwXCountChars = 0;
    si.dwXSize = 0;
    si.dwY = 0;
    si.dwYCountChars = 0;
    si.dwYSize = 0;
    si.dwY = 0;
    si.dwYCountChars = 0;
    si.hStdError = NULL;
    si.hStdInput = NULL;
    si.hStdOutput = NULL;
    si.lpDesktop = NULL;
    si.lpReserved = NULL;
    si.lpReserved2 = NULL;
    si.lpTitle = NULL;
    si.wShowWindow = 0;
    */

    strcat(exe, line);
    if (!CreateProcess(NULL, exe, NULL, NULL, 0, 0, NULL, NULL, &si, &pi)) {
       fprintf(stdout, "Invalid Command provided. Run the <help> command for more information.\n");
       return; 
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

    return;
}

