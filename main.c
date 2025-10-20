#include <stdio.h>
#include <unistd.h>     // for fork(), pipe()
#include <sys/wait.h>   // for waitpid()
#include <string.h>     // for strlen()
#include "Config/MakeConfig.h"
#include "Errors/errors.h"

int main(int argc , char * argv[]){
     int fd[2];
     pid_t pid ;

     if (pipe(fd) == -1) {
          forkCreationProblem();
          return 1;
     }

     pid = fork() ; 
     
     if (pid < 0){
          forkCreationProblem();
          return 1 ; 
     }
     else if (pid == 0) {
          // Child 
          if (argc <= 1){
               invalidCommand(); 
          }
          if (argv[1] == "config"){
               SecondAgrumentChecking(argv);
          }

     }
     else {
          close(fd[0]); // Close read end (parent will write)
          int status; 
          waitpid(pid , &status , 0);
     }
     

     return 0;
}