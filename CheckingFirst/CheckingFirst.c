#include <stdio.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "../Errors/errors.h"

#define BUFFER_SIZE 256

int isUserConfig(){
     char * home = getenv("HOME");

     if (home == NULL) {
          forkCreationProblem();
          return;
     }
     char filePath[512];
     snprintf(filePath, sizeof(filePath), "%s/NewGit2.0/configUser.txt", home);

     FILE *file = fopen(filePath, "a");
     if (file == NULL) {
          fileCreationConfigError();
          return;
     }

     int count = 0;
     char buffer[BUFFER_SIZE];
     while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
          count ++ ;
     }

     if (count < 2) {
          return 0;
     }
     return 1;
}

bool fileExists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

bool dirExists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

bool isInit() {
     const char *filePath = ".newgit/idInfo.txt";
     const char *dirPath  = ".newgit/StagingInfo";

     if (!fileExists(filePath) || !dirExists(dirPath)){
          return false;
     }
     return true ;
    
}

bool PwdChecking(char * pwd) {
     char * home = getenv("HOME");

     if (home == NULL) {
          forkCreationProblem();
          return;
     }
     char filePath[512];
     snprintf(filePath, sizeof(filePath), "%s/NewGit2.0/InitInfo.txt", home);

     FILE *file = fopen(filePath, "a");
     if (file == NULL) {
          fileCreationConfigError();
          return;
     }
     char buffer[BUFFER_SIZE];
     while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
          if (buffer == pwd) {
               return 1;
          }
     }
     return 0;

}

char *gettingPwd() {
     char *cwd = getcwd(NULL, 0);  // getcwd() allocates memory when first arg = NULL
     if (cwd == NULL) {
          // Print why getcwd failed
          fprintf(stderr, "Error: getcwd failed - %s\n", strerror(errno));
          return NULL;
     }
     return cwd;  // caller must free() this
}

int task() {
     if (!isUserConfig()) {
          return 1;
     } 
     char * pwd = gettingPwd();

     if (!isInit()) {
          return 2 ;
     }

     if (!PwdChecking(pwd)) {
          return 3;
     }
     return 4;
}