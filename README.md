# my_shell
### About:
This is a personal project where I built a shell from scratch. If you are interested in the implementation details you can read the files provided.
For more information about what a shell is and its role in computing check this link: https://en.wikipedia.org/wiki/Shell_(computing)
### Implementation:
The main fucntion for the shell program can be found in 'my_shell.c', where it simulates what a general shell should do no matter what operating system is being run on. 
The way I implemented the shell is by dividing the process into three major steps:

  - Reading the command
  - Parsing the command
  - Executing the command
  
 The problem is that this three steps are different in the way they are implemented depending on the system.
 Thus, I provided three header files for the following operating systems: Windows, MacOS, and Linux.
 ### Compilation:
In the repository I have already included two executable files 'my_shell_Windows.exe' for Windows and 'my_shell_MacOS' for MacOS that are ready to go and to use. These two shells programs will have the same functioinality as a default shell (e.g. Command Prompt or cmd for Windows). 

Now, if you want to add custom shell commands or to expand the functionality of the shell you will have to compile the code. 
In order to do that you will need two files: 'my_shell.c' and the corresponding header file for the operating system you are going to compile it on.

Once, you have downloaded the two files you are ready to compile.
e.g: gcc my_shell.c -o name_of_your_shell

 
 

