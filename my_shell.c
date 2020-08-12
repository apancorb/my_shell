#ifdef _WIN32
	#include "windows_my_shell.h"
#elif __linux__
	#include "linux_my_shell.h"
#elif __APPLE__
    #include "macos_my_shell.h"
#endif


int main (int argc, char *argv[]) {
    /* input: variable that represents where the user's input is going to come from. */
    FILE *input;
    /* line: variable that represents the entire command the user has wrote. */
    char line[VALID_LINE] = {0};
    /* my_cmd: variable that represents the name of the shell, feel free to change it. */
    const char *const shell_name = "my_shell:";

    /* 

    We check were the user wants the input to be coming from:
        The two options are: 
            1. standard input
            2. file (preferably .txt)

     */

    if (argc == 1) { /* standard input from the user */
        input = stdin; 
    } else if (argc == 2){ /* standard input comes from the file the user provided */
        if((input = fopen(argv[1], "r")) == NULL){
            fprintf(stderr, "%s cannot be opened.", argv[1]);
            return EXIT_FAILURE;
        }
    } else { /* if more than two arguments on the command line */
      fprintf(stderr, "Amount of arguments exceeded\n");
      fprintf(stdout, "Amount of arguments passed in have been exceeded. Run the <help> command for more information.\n");
      return EXIT_FAILURE;      
   }
    /* 

    The way the shell is going to work is as follows:
        1. Read the command
        2. Parse the command
        3. Exexute the command
    This proccess will be repeated until the user manually enters the command <quit> or <exit> or 
    uses a signal to terminate the shell.

    */

    fprintf(stdout, "%s ", shell_name);
    fflush(stdout);

    /* Read the command, the command must be less than 1024 characters */
    while (fgets(line, VALID_LINE, input) != NULL) {
        int is_process;

        if ((is_process = process(line, input)) == FALSE) {
            /* the shell doesn't terminate when there is an invalid command */
            fprintf(stdout, "%s ", shell_name);
            fflush(stdout);
            continue;
        }

        fprintf(stdout, "%s ", shell_name);
        fflush(stdout);
    }

    return 0;
    
}


      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      /*  char first_string[VALID_LINE] = {0};
        int values_read = 0, i = 0, is_blank_line = 0;


        if(input == stdin){
            printf("> ");
        }
        
        /* check if it is a blank line 
        for(i = 0; i < strlen(line); i++){
            if(isspace(line[i]) != 0){
                is_blank_line++;
            }
        }
        if(is_blank_line == strlen(line)){
            continue;
        }
        
        /* read the first string in the line 
        sscanf(line, " %s", first_string)*/