#include <stdio.h>
#include <unistd.h>     // for fork(), pipe()
#include <sys/wait.h>   // for waitpid()
#include <string.h>     // for strlen()
#include <errno.h>    // For errno
#include <unistd.h>
#include "initialization.h"
#include "errors.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"

void makingDotGitFolder(){
     const char * folderName = ".git";
     mode_t  permissions  = 0755 ;

     if (mkdir(folderName , permissions) == 0) {
          sleep(3);
          makingStagingIdInfoFile();
     }
     else {
          forkCreationProblem() ;
     }
     return ;
}

void makingStagingIdInfoFile() {
     const char * filePath = ".git/idInfo.txt";

     FILE * file = fopen(filePath , "w");

     if (file == NULL) {
          fileCreationErrorInDotGit() ;
          return ;
     }
     fclose(file);


}

void makingStagingIdsFolder() {
     const char * folderName = ".git/StagingInfo";
     mode_t  permissions  = 0755 ;

     if (mkdir(folderName , permissions) == 0) {
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