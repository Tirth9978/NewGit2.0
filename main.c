#include <stdio.h>
#include <unistd.h>     // for fork(), pipe()
#include <sys/wait.h>   // for waitpid()
#include <string.h>     // for strlen()
#include "Config/MakeConfig.h"
#include "Errors/errors.h"
#include "Staging/staging.h"
#include "Revert/revert.h"
#include "CommitInfo/info.h"
#include "CheckingFirst/CheckingFirst.h"


#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"

#define VERSION "1.0.1"

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

          // ---- CHILD PROCESS ----
          close(fd[1]); // Close write end

          // First read how many arguments were sent
          int arg_count;
          read(fd[0], &arg_count, sizeof(int));

          // Allocate array to store arguments
          char **child_argv = malloc((arg_count + 1) * sizeof(char *));
          if (!child_argv) {
               perror("malloc failed");
               return 1;
          }

          // Read each argument string
          for (int i = 0; i < arg_count; i++) {
               int len;
               read(fd[0], &len, sizeof(int)); // length of string
               child_argv[i] = malloc(len + 1);
               read(fd[0], child_argv[i], len);
               child_argv[i][len] = '\0';
          }
          child_argv[arg_count] = NULL; // end of argv[]

          close(fd[0]);
          
          char * arg1 = argv[1];
          char * arg2 = argv[2];
          char * arg3 = argv[3];
          char * arg4 = argv[4];
     
     
          if (arg_count >= 1 && strcmp(child_argv[0], "config") == 0) {
               SecondAgrumentChecking(arg1 , arg2 , arg3 , arg4);
          }
          else if (arg_count >= 1 && strcmp(child_argv[0] , "--version") == 0) {
               
               printf( CYN "NewGit2.0 --->" VERSION "\n" END );
               printf( YEL "Thank You for using NewGit2.0\n" END );
               if(task()==1){
                    printf(RED "User Not config :(\n" END);
                    printf(GRN "Just Do this : \n" END);
                    printf(YEL "newgit config --global user.name <User Name>\n" END);
                    printf(YEL "newgit config --global user.email <Email ID>\n" END);
               }
          }
          else if (arg_count >= 1 && strcmp(child_argv[0] , "init") == 0) {
               if (task()==1) {
                    printf(RED "User Not config :(\n" END);
                    printf(GRN "Just Do this : \n" END);
                    printf(YEL "newgit config --global user.name <User Name>\n" END);
                    printf(YEL "newgit config --global user.email <Email ID>\n" END);
                    return 3;
               }
               makingDotGitFolder();
          }
          else if (arg_count >= 1 && strcmp(child_argv[0],"--info") == 0) {
               if (task()==1) {
                    printf(RED "User Not config :(\n" END);
                    printf(GRN "Just Do this : \n" END);
                    printf(YEL "newgit config --global user.name <User Name>\n" END);
                    printf(YEL "newgit config --global user.email <Email ID>\n" END);
                    return 3;
               }
               if (task() == 2 || task() == 3) {
                    printf(RED "Repo is not Initialised :(\n" END);
                    return 4;
               }
               printf("Info Taker");
               gettingInfo();
          }
          else if (arg_count >= 1 && strcmp(child_argv[0] , "add") == 0 && strcmp(child_argv[1] , ".") == 0){
               if (task()==1) {
                    printf(RED "User Not config :(\n" END);
                    printf(GRN "Just Do this : \n" END);
                    printf(YEL "newgit config --global user.name <User Name>\n" END);
                    printf(YEL "newgit config --global user.email <Email ID>\n" END);
                    return 3;
               }
               if (task() == 2 || task() == 3) {
                    printf("%d\n" , task());
                    printf(RED "Repo is not Initialised :(\n" END);
                    return 4;
               }
               addingStaging();
          }
          else if (arg_count >= 1 && strcmp(child_argv[0],"commit") == 0 ){
               if (task()==1) {
                    printf(RED "User Not config :(\n" END);
                    printf(GRN "Just Do this : \n" END);
                    printf(YEL "newgit config --global user.name <User Name>\n" END);
                    printf(YEL "newgit config --global user.email <Email ID>\n" END);
                    return 3;
               }
               if (task() == 2 || task() == 3) {
                    printf(RED "Repo is not Initialised :(\n" END);
                    return 4;
               }
               if (strcmp(child_argv[1] , "-m")==0) {
                    if (child_argv[2] == NULL) {
                         ForGotMessage() ;
                    }
                    else {
                         char * message = child_argv[2];
                         appendMessage(message);
                    }
               }
               else {
                    Forgot_M();
               }
          }
          else if (arg_count >= 1 && strcmp(child_argv[0],"revert") == 0) {
               if (task()==1) {
                    printf(RED "User Not config :(\n" END);
                    printf(GRN "Just Do this : \n" END);
                    printf(YEL "newgit config --global user.name <User Name>\n" END);
                    printf(YEL "newgit config --global user.email <Email ID>\n" END);
                    return 3;
               }
               if (task() == 2 || task() == 3) {
                    printf(RED "Repo is not Initialised :(\n" END);
                    return 4;
               }
               if (child_argv[1] == NULL) {
                    idNotFound();
               }
               else {
                    char * id = child_argv[1];
                    printf("In main : %s\n",id);
                    revertById(id);
               }
          }
          else if (arg_count >= 1 && strcmp(child_argv[0],"rm") == 0 && strcmp(child_argv[1],"--info") == 0) {
               removeHistory();
          }
          
          else {
               invalidCommand();
          }
          exit(1);
     }
     else {
          close(fd[0]); // Close read end

          // Send total number of arguments (excluding program name)
          int arg_count = argc - 1;
          write(fd[1], &arg_count, sizeof(int));

          for (int i = 1; i < argc; i++) {
               int len = strlen(argv[i]);
               write(fd[1], &len, sizeof(int));
               write(fd[1], argv[i], len);
          }
          close(fd[1]);
          int status; 
          waitpid(pid , &status , 0);
     }
     

     return 0;
}