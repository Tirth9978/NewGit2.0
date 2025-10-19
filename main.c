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
          if (argv[1] == "config"){
               SecondAgrumentChecking(argv)
          }

          // if (findMainConfigFile()){
          //      printf("Ok\n");
          // }
          // else {
          //      printf("No\n");
          // }
     }
     else {
          int status ; 
          waitpid(pid , &status , 0);
     }
     

     return 0;
}