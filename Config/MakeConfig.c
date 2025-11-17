#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for fork(), usleep()
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


#define VERSION "1.0.1"


// ===================== Spinner Animation 3 =====================
void spinnerAnimation3(const char *message, int cycles, int delayMs) {
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
          printf(BLU "====================================\n" END);
          printf(BLU "        NewGit2.0 Config Tool       \n" END);
          printf(BLU "====================================\n\n" END);

          spinnerAnimation3(CYN "Processing global configuration", 15, 60);

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
                         spinnerAnimation3(CYN "Saving user.name", 12, 70);
                         addConfigUser(arg4 , "Username") ;
                    }
               }
               else if (strcmp(arg3 , "user.email") == 0){
                    if (arg4 == NULL){
                         NotFoundUserName();
                    }
                    else {
                         if (strstr(arg4 , "@") != NULL && (strstr(arg4 , ".com") != NULL || strstr(arg4 , ".in") != NULL)){
                              spinnerAnimation3(CYN "Saving user.email", 12, 70);
                              addConfigUser(arg4 , "Email") ;
                         }
                         else {
                              printf( RED "Invalid Email type\n" END );
                         }
                    }
               }
               else {
                    spinnerAnimation3(CYN "Updating custom config key", 12, 70);
                    userConfig(arg3);
               }
          }
          else {
               int status ;
               waitpid(pid , &status , 0);
          }
     }
     else if (strcmp(arg2 , "--info") == 0) {
          printf(BLU "====================================\n" END);
          printf(BLU "        NewGit2.0 Config Info       \n" END);
          printf(BLU "====================================\n\n" END);

          spinnerAnimation3(CYN "Fetching configuration info", 15, 60);

          pid = fork() ;
          if (pid < 0){
               forkCreationProblem();
               exit(1);
          }
          if (pid == 0){
               ReadingConfigData();
          }
          else {
               int status ;
               waitpid(pid , &status , 0);
          }
     }
     else {
          printf(RED "Sorry :( Unknown config option.\n" END);
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

void addConfigUser(char *str, char *type) {
    char *home = getenv("HOME");
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

    char message[256];

    if (strcmp(type, "Username") == 0) {
         snprintf(message, sizeof(message), "Username :%s\n", str);
    } else if (strcmp(type, "Email") == 0) {
         snprintf(message, sizeof(message), "Email    :%s\n", str);
    } else {
         snprintf(message, sizeof(message), "%s :%s\n", type, str);
    }

    size_t bytes_written = fwrite(message, sizeof(char), strlen(message), file);

    if (bytes_written != strlen(message)) {
        fileCreationConfigError();
        fclose(file);
        return;
    }

    fclose(file);

    spinnerAnimation3(GRN "Finalizing configuration", 10, 70);

    printf(GRN "%s successfully configured as %s\n" END, type, str);
    printf(YEL "Thank You for using NewGit2.0\n" END);
    printf(CYN "NewGit2.0 ---" VERSION END "\n");
    return;
}

void ReadingConfigData() {
     char* home = getenv("HOME");

     if (home == NULL){
          forkCreationProblem();
          return;
     }

     char filePath[512];
     snprintf(filePath, sizeof(filePath), "%s/NewGit2.0/configUser.txt", home);

     FILE * file = fopen(filePath, "r");

     if (file == NULL) {
          printf(RED "Error: Could not open configuration file at %s\n" END, filePath);
          return;
     }

     printf(GRN "Current NewGit2.0 global configuration:\n\n" END);

     char line[256];
     while (fgets(line, sizeof(line), file) != NULL) {
          // Remove trailing newline character if present
          line[strcspn(line, "\n")] = '\0';
          printf(WHT "%s\n" END, line);
     }
     printf("\n");
     printf(YEL "Thank You for using NewGit2.0\n" END);
     printf(CYN "NewGit2.0 ---" VERSION "\n" END);
     fclose(file);
}
