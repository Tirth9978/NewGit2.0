#include <stdio.h>
#include <unistd.h>     // for fork(), pipe()
#include <sys/wait.h>   // for waitpid()
#include <string.h>     // for strlen()
#include <errno.h>    // For errno
#include <unistd.h>
#include<stdlib.h>
#include "initialization.h"
#include "../Errors/errors.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"

char * gettingPwd() {
     char *cwd = getcwd(NULL, 0);  
     return cwd;  
}

void makingDotGitFolder(){
     const char * folderName = ".newgit";
     if (mkdir(folderName , 0755) == 0) {
          sleep(3);
          makingStagingIdInfoFile();
     }
     else {
          forkCreationProblem() ;
     }
     return ;
}

void makingStagingIdInfoFile() {
     char *current_dir = gettingPwd();

     if (current_dir == NULL) {
          ProblemInInit();
          return ; 
     }
     const char * filePath = ".newgit/idInfo.txt";
     FILE * file = fopen(filePath , "w");

     if (file == NULL) {
          fileCreationErrorInDotGit() ;
          return ;
     }
     fclose(file);
     
     makingInitInfoFile(current_dir);
     makingStagingIdsFolder();
     return ;
}

void makingStagingIdsFolder() {
     const char * folderName = ".newgit/StagingInfo";

     if (mkdir(folderName , 0755) == 0) {
          sleep(3);
          printf( GRN "Initialized Empty Git Repo\n" END);
          printf( YEL "Thnak You using NewGit2.0\n" END );
          printf(CYN "NewGit2.0 --> 1.0.1\n" END);
     }
     else {
          forkCreationProblem() ;
     }
     return ;
}

void makingInitInfoFile(char * pwd) {
     const char * home = getenv("HOME");

     if (!home){
          ProblemInInit();
          return ;
     }
          
     char file_path[256];
     snprintf(file_path, sizeof(file_path), "%s/NewGit2.0/InitInfo.txt", home);

     FILE * file = fopen(file_path , "a");

     if (!file) {
          ProblemInInit();
          return ;
     }

     fprintf(file, "%s\n" , pwd); // Add a newline after "Hello"

     // Close the file
     fclose(file);
     return ;
}