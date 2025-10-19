#include <stdio.h>
#include <unistd.h>     // for fork()
#include <sys/wait.h>   // for wait()
#include "Config/MakeConfig.h"
#include "Errors/errors.h"

int main(int argc , char * argv[]){

     pid_t pid ;

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
     }
     else {
          int status ; 
          waitpid(pid , &status , 0)
          printf("Parent: Child process finished handling invalid command.\n");
     }
     

     return 0;
}