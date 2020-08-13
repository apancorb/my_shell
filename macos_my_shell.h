#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h> 
#include <unistd.h>

#define VALID_LINE 1024
#define TRUE 0
#define FALSE 1

typedef struct Cmd {
    char *cmd_name;
    struct Cmd *next;
} Cmd;

typedef struct Cmd_List {
    struct Cmd *head;
    int words;
} Cmd_List;

int process(char *line) {
    int words = 0, i = 0;
    /* we start to process the command line if it is not an empty line */
    if ((words = count_num_cmd(line)) != 0) {
        /* list: variable that represents a singly linked list where commands will be stored */
        Cmd_List *list;
        /* curr: variable that represents the current node in the list */
        Cmd *curr;
        /* parsed_line: output after parsing the given command line */
        char *parsed_line[words];
        /* is_success: boolean variable */
        int is_success;
        /* Initialize the linked list */
        if ((is_success = init_cmd_list(&list, words)) == FALSE) {
            fprintf(stderr, "init_cmd_list error");
            fprintf(stdout, "Something went wrong internally. Run the <help> command for more information.\n");
            exit(EXIT_FAILURE);
        }
            
	
        /* parse the command line */
        parse(line, words, list); 
        /* add each command line to a string of pointers, that will be used for the exucation */
        curr = list->head;
        for (i = 0; i < words; i++) {
            parsed_line[i] = curr->cmd_name;
            curr = curr->next;
        }
        parsed_line[words] = NULL;
        /* execute the command line */
        if ((is_success = execute(parsed_line, words)) == FALSE) {
            /* note: that the shell keeps running even if the user provided invalid commands */
            return FALSE;
        }
        destroy_cmd_list(list);
        return TRUE;
    } else {
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

/* returns a parsed line, by the out of parameter, using the line provided by the user */
parse(char * line, int words, Cmd_List *list) {
    int i = 0, j = 0, l = 0;
    int is_success;
    char word[VALID_LINE] = {0};

    while (line[i] != '\0') {
        if (isspace(line[i]) == 0) {
            j = 0;
            while (isspace(line[i]) == 0) {
                word[j++] = line[i];
                i += 1;
            }
            /* We add the word to our linked list */
            if ((is_success = add_cmd(list, word)) == FALSE) {
                fprintf(stderr, "add_cmd error");
                fprintf(stdout, "Something went wrong while parsing your command. Run the <info> command for more information.\n");
                exit(EXIT_FAILURE);
            }
            while (j-- != 0) {
                word[j] = '\0';
            }
        } else {
            i += 1;
        } 
        
    }
}

/* initializes the list of commands */ 
int init_cmd_list(Cmd_List **list, int words) {
    *list = malloc(sizeof(Cmd_List));
    if (*list == NULL) {
        return FALSE;
    }
    (*list)->words = words;
    (*list)->head = NULL;
    return TRUE;
}

int add_cmd(Cmd_List *list, char *word) {
    Cmd *command, *curr;
    /* First we check if the parameters are correct */
    if (list == NULL || word[0] == '\0') {
        return FALSE;
    }
    command = malloc(sizeof(Cmd));
    if (command == NULL) {
        return FALSE;
    }
    command->cmd_name = malloc(strlen(word) + 1);
    if (command->cmd_name == NULL) {
        return FALSE;
    }
    strcpy(command->cmd_name, word);
    command->next = NULL;

    curr = list->head;
    if (curr == NULL) {
        list->head = command;
    } else {
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = command;
    }
    return TRUE;
} 

void destroy_cmd(Cmd *command) {
    free(command->cmd_name);
    free(command);
}

destroy_cmd_list(Cmd_List *list) {
    int i = 0;
    Cmd *curr = list->head;
    for (i = 0; i < list->words; i++) {
        destroy_cmd(curr);
        curr = curr->next;
    }
    free(list);
}

int execute(char **parsed, int words) {
    int is_success;
    /* 
        There are two type of commands:
            1. Shell (cd, quit, sql, and any other command you want the shell to do)
            2. Non-Shell
    */
    if ((strcmp(parsed[0], "quit") == 0 || strcmp(parsed[0], "exit") == 0) && words == 1) {
        exit(EXIT_SUCCESS);
    } else if (strcmp(parsed[0], "cd") == 0) {
        if ((is_success = execute_cd(parsed, words)) == FALSE) {
            fprintf(stdout, "There was an error changing directory. Run the <info> command for more information.\n");
            return FALSE;
        }
    /* } else if (strcmp(parsed[0], "sql") == 0 ) {
        execute_sql(parsed); */
    } else {
       execute_non_shell(parsed, words); 

    }
  
}

int execute_cd(char **parsed, int words) { 
    /* if we need to go to the home dir */
    if (words == 1) {
        char *loc = getenv("HOME");
        int is_chdir_success = chdir(loc);
        if (is_chdir_success == -1) {
            perror(loc);
            return FALSE; /* since the command was unsuccesful */
        }
    } else if (words == 2) { /* else we got to the specified dir using the second argumnet of the command */
        int is_chdir_success = chdir(parsed[1]);
        if (is_chdir_success == -1) {
            perror(parsed[1]);
            return FALSE; /* since the command was unsuccesful */
        }
    } else { /* Inavalid Command */
        fprintf(stderr, "cd error");
        fprintf(stdout, "Invalid Command, too many arguments for <cd>. Run the <info> command for more information.\n");
        return FALSE;
    }
}

int execute_non_shell(char **parsed, int words) {
    pid_t pid; 
    /* we fork the parent */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(EXIT_FAILURE); 
    } 
    
     /* child executing the command */
    if (pid == 0) { 
        execvp(parsed[0], (char * const *)parsed);
        /* since the command was unsuccesful */
        fprintf(stderr, "Failed to execute %s\n", parsed[0]);
        exit(EXIT_FAILURE);
        /* parent executing the command */   
    } else {    
        int status = 0;
        wait(&status); 
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 0) {
                return TRUE; /* since the command was successful */
            } else {
                return FALSE; /* since the command was unsuccessful */
            }
        }
    }   
}