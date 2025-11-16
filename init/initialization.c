#include <stdio.h>
#include <unistd.h>     // for fork(), pipe(), sleep(), usleep()
#include <sys/wait.h>   // for waitpid()
#include <string.h>     // for strlen()
#include <errno.h>      // For errno
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>  
#include <pwd.h>
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

#define VERSION "1.0.1"

// Simple spinner animation
void spinnerAnimation(const char *message, int cycles, int delayMs) {
     const char spinnerChars[] = "|/-\\";
     int spinnerLen = 4;

     printf("%s ", message);
     fflush(stdout);

     for (int i = 0; i < cycles; i++) {
          char c = spinnerChars[i % spinnerLen];
          printf("%c", c);
          fflush(stdout);
          usleep(delayMs * 1000); // delayMs milliseconds
          printf("\b"); // move cursor back
     }
     printf(" \n"); // clear char & go next line
}

char *gettingPwd() {
     char *cwd = getcwd(NULL, 0);  // getcwd() allocates memory when first arg = NULL
     if (cwd == NULL) {
          // Print why getcwd failed
          fprintf(stderr, RED "Error: getcwd failed - %s\n" END, strerror(errno));
          return NULL;
     }
     return cwd;  // caller must free() this
}

bool checkInMainFolder(char * pwd) {
     if (pwd == NULL) {
          return 0;
     }

     const char * home = getenv("HOME");
    
     if (home == NULL) {
          struct passwd *pw = getpwuid(getuid());
          if (pw != NULL) {
               home = pw->pw_dir;
          }
     }
    
     char file_path[1024];
     snprintf(file_path, sizeof(file_path), "%s/NewGit2.0/InitInfo.txt", home);

     FILE * file = fopen(file_path , "r");
  
     if (file == NULL){
          forkCreationProblem();
          return 0;
     }
  
     char line[1024];
     bool found = false;

     while (fgets(line, sizeof(line), file)) {
          // Remove newline character at the end
          size_t len = strlen(line);
          if (len > 0 && line[len - 1] == '\n') {
               line[len - 1] = '\0';
          }

          // Compare with the PWD we're checking
          if (strcmp(line, pwd) == 0) {
               found = true;
               break;
          }
     }

     fclose(file); // Closing the file is important :)
 
     return found;
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

     fprintf(file, "%s\n" , pwd);

     fclose(file);
     return ;
}

void makingStagingIdsFolder() {
     const char * folderName = ".newgit/StagingInfo";

     spinnerAnimation(CYN "Creating staging area", 12, 100); // ~1.2 sec

     if (mkdir(folderName , 0755) == 0) {
          sleep(1);
          printf(GRN "\nInitialized empty NewGit repo in this directory.\n" END);
          printf(YEL "Thank you for using NewGit2.0\n" END);
          printf(CYN "NewGit2.0 --> " VERSION "\n" END);
     }
     else {
          forkCreationProblem();
     }
     return ;
}

void makingStagingIdInfoFile(char * pwd) {
     
     printf(CYN "\nSetting up internal files...\n" END);

     spinnerAnimation("Creating .newgit/idInfo.txt", 10, 80);

     if (pwd == NULL) {
          ProblemInInit();
          return ; 
     }

     const char * filePath = ".newgit/idInfo.txt";
     FILE * file = fopen(filePath , "w");

     if (file == NULL) {
          fileCreationErrorInDotGit();
          return ;
     }
     fclose(file);
     
     spinnerAnimation("Updating InitInfo.txt", 10, 80);
     makingInitInfoFile(pwd);

     makingStagingIdsFolder();
     return ;
}

void makingDotGitFolder(){
     printf(BLU "===============================\n" END);
     printf(BLU "     NewGit2.0 Initializer     \n" END);
     printf(BLU "===============================\n\n" END);

     char *current_dir = gettingPwd();
     if (!current_dir) {
          return;
     }

     printf(CYN "Current Directory: " WHT "%s\n" END, current_dir);

     spinnerAnimation(CYN "Checking if this repo is already initialized", 15, 70);

     if (checkInMainFolder(current_dir)) {
          printf(GRN "\nThis repo is already initialized :)\n" END);
          printf(CYN "NewGit2.0 ---> " VERSION "\n" END);
          free(current_dir);
          return ;
     }

     printf(YEL "\nInitializing NewGit repository...\n" END);
     spinnerAnimation("Creating .newgit folder", 15, 70);

     const char * folderName = ".newgit";
     if (mkdir(folderName , 0755) == 0) {
          sleep(1);
          makingStagingIdInfoFile(current_dir);
     }
     else {
          forkCreationProblem();
     }

     free(current_dir);
     return ;
}
