#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//adds log statements into the log file (when child process terminates)
void log_handler(){
	
	FILE *p;
        p = fopen("log.txt", "a");
        if(p == NULL)
         printf("Error opening file.");
        else 
        fprintf(p, "--> child process terminated.\n");
        fclose(p);
}

int main()
{
   while(1)
   {
    char input[256];
    char* parsed;
    char* command[20];
    int index = 0;
    int background = 0;
    
    //receives a signal when child process terminates and calls the log handler function
    signal(SIGCHLD, log_handler); 
    
    printf("Shell>");
    fgets(input, 256, stdin);
    
    // removes the newline & puts NULL as end of input string
    if ((input[strlen (input) - 1] == '\n'))
        input[strlen (input) - 1] = '\0';

       
   ///if command is empty
    if(strcmp(input, "") == 0)
    {
      continue;
    }
   
    parsed = strtok(input, " ");
    while(parsed != NULL)
    {
        // checks for "&" in the command & breaks the loop if found
        if(strcmp(parsed, "&") == 0)
        {
            background = 1;
            break;
        }
        command[index] = parsed;
        index++;
        parsed = strtok(NULL, " ");
    }

     command[index] = NULL;   //puts NULL after the last argument in the command 
 
    
     //if command is exit
     if(strcmp(command[0],"exit") == 0)
      {
       printf("Exiting the shell....\n");
       exit(0);
      }
    
    //if command is cd
    if(strcmp(command[0],"cd") == 0)
    {
      if(chdir(command[1]) < 0) //executes cd
      {
        printf("\nWrong Directory!\n"); //if returned value = -1 then the directory doesn't exist
        
      }
      continue; //skips the fork
    }
     
    
     //creating a child process via fork()
    pid_t pid = fork();  
    if (pid < 0)  
    {
      printf("Failed to create child process!");
    } 
    else if(pid == 0)  //child process
    {
      if(execvp(command[0], command) < 0)  //executes the command and checks the return value
      {
         printf("ERROR: Failed to execute");
      }
    }
    else     //pid > 0 parent process
    {
      
      if(!background)       // "&" was not entered 
      wait(NULL);            // wait for child process to finish
    }
 }
     
return 0;
}
