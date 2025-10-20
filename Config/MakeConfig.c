#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for fork()
#include <sys/wait.h>   // for wait()
#include "MakeConfig.h"
#include "../Errors/errors.h"


#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"

int findMainConfigFile (){
     const char *homeDir = getenv("HOME");

     if (!homeDir){
          printf(RED "Error: HOME environment variable not set.\n" END) ;
          return 1;
     }

     char * filePath = malloc(strlen(homeDir) + strlen("/NewGitConfig") + 1);

     if (!filePath){
          printf(RED "Memory allocation failed.\n" END);
          return 2;
     }
     sprintf(filePath, "%s/NewGitConfig", homeDir);

     FILE * file = fopen(filePath , "r");

     if (file){
          
          fclose(file);
          free(filePath);
          return 1;
     }
     else {
          free(filePath);
          return 0;
     }
}

void SecondAgrumentChecking(char * argv[]) {
     pid_t pid ;
     printf("Yes I am in :)");
     printf("%s" , argv[2])
     if (argv[2] == "--global"){
          pid = fork();
          if (pid < 0){
               forkCreationProblem();
               exit(1);
          }
          printf("Yes I am in --global:)");
          if (pid == 0){
               if (argv[3] == "user.name"){
                    if (argv[4] == NULL){
                         NotFoundUserName();
                    }
                    else {
                         if (!findMainConfigFile()){
                              makeFile();
                              printf("Yes I am in username:)");
                         }
                    }
               }
               if (argv[3] == "user.email"){
                    if (argv[4] == NULL){
                         NotFoundUserName();
                    }
                    else {
                         if (!findMainConfigFile()){

                         }
                    }
               }
               else {
                    userConfig(argv[3]);
               }
          }
          else {
               int status ;
               waitpid(pid , &status , 0);
          }
     }
     else {
          printf("Sorry :(");
     }
}

void makeFile() {
     const char *home_dir = getenv("HOME");
     if (home_dir == NULL) {
        forkCreationProblem();
        return ;
     }

     char path[1024];
     snprintf(path, sizeof(path), "%s/NewGit2.0", home_dir);
     if (mkdir(path, 0755) == -1) {
          perror("Error creating directory");
          return ;
     }
     return ;
}