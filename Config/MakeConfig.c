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

void SecondAgrumentChecking(char * arg1 , char * arg2 , char * arg3 , char * arg4) {
     pid_t pid ;
     
     if (strcmp(arg2 , "--global") == 0){
          pid = fork();
          if (pid < 0){
               forkCreationProblem();
               exit(1);
          }
          if (pid == 0){
               if (strcmp(arg3 , "user.name") == 0){
                    if (arg4 == NULL){
                         NotFoundUserName();
                    }
                    else {
                         // if (!findMainConfigFile()){
                         //      makeFile();
                         // }
                         addConfigUser(arg4 , "username") ;
                    }
               }
               else if (arg3 == "user.email"){
                    if (arg4 == NULL){
                         NotFoundUserName();
                    }
                    else {
                         if (!findMainConfigFile()){

                         }
                    }
               }
               else {
                    userConfig(arg3);
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

void addConfigUser(char * str , char * type) {
     char * home = getenv("HOME");
     if (home == NULL){
          forkCreationProblem() ; 
          return ;
     }
     char filePath[512];
     vsnprintf(filePath, sizeof(filePath), "%s/NewGit2.0/configUser.txt", home);

     FILE * file = fopen(filePath , 'a');
     if (file == NULL){
          fileCreationConfigError();
          return ;
     }
     char * message = "Username :";
     strcat(message , str);

     size_t bytes_written = fwrite(message , sizeof(char) , strlen(message) , file);

     if (bytes_written != strlen(message)){
          fileCreationConfigError();
          return ;
     }

     fclose(file);

     printf(GRN "%s ,Successfully config as %s\n" , type , str, END);
     printf( YEL "Thank You for using NewGit2.0\n" END);
     printf( CYN "NewGit2.0 --- 1.0.1\n" END );
     return ;
}