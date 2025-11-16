#include <stdio.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "commit.h"
#include "../Errors/errors.h"
#include "../CheckingFirst/CheckingFirst.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"

#define VERSION "1.0.1"

//Structures
struct FileCount{
     int files ; 
     int folders;
};

// ===================== Spinner Animation 2 =====================
void spinnerAnimation2(const char *message, int cycles, int delayMs) {
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

// Only For the Counting part not for the Exporting 
// CurrDIR 
struct FileCount countCurrentDir() {
     struct FileCount count = {-1 , -1 };
     DIR *dir;
     struct dirent *entry;
     struct stat statbuf;

     dir = opendir(".");

     if (dir == NULL) {
          printf("Error opening current directory\n");
          return count;
     }
     count.files = 0;
     count.folders = 0;

     while ((entry = readdir(dir)) != NULL) {
          // Skip hidden files and .newgit directory
          if (entry->d_name[0] == '.' || strcmp(entry->d_name, ".newgit") == 0) {
               continue;
          }
          
          // Get file status
          if (stat(entry->d_name, &statbuf) == -1) {
               continue;
          }
          
          if (S_ISDIR(statbuf.st_mode)) {
               count.folders++;
          } else {
               count.files++;
          }
     }

     closedir(dir);
     return count;
}

struct FileCount countFilesInPath(const char *basePath) {
    struct FileCount count = {0, 0};
    struct dirent *dp;
    struct stat st;

    DIR *dir = opendir(basePath);
    if (!dir)
        return count;

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", basePath, dp->d_name);

        if (stat(fullPath, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                count.folders++;
                struct FileCount subCount = countFilesInPath(fullPath);
                count.files += subCount.files;
                count.folders += subCount.folders;
            } else if (S_ISREG(st.st_mode)) {
                count.files++;
            }
        }
    }

    closedir(dir);
    return count;
}

struct FileCount countInStagDIR(char * id ) {
     char path[256];
     snprintf(path, sizeof(path), ".newgit/StagingInfo/%s", id);
     return countFilesInPath(path);
}

char *getingId() {
     const char *path = ".newgit/idInfo.txt";
     FILE *fp;
     char line[200];
     char ids[100][100];  // store all IDs
     int count = 0;
     static char result[100]; // static so it's safe to return

     fp = fopen(path, "r");  // open the file
     if (fp == NULL) {
          printf("Error opening file!\n");
          // You can call problemInCommit() here if needed
          return "ERROR";
     }

     while (fgets(line, sizeof(line), fp)) {
          // check if line starts with "Id:"
          if (strncmp(line, "Id:", 3) == 0) {
               char temp[100];
               // extract the id value after "Id:"
               if (sscanf(line, "Id:%s", temp) == 1 && strlen(temp) > 0) {
                    strcpy(ids[count++], temp);
               }
          }
     }

     fclose(fp);

     if (count == 0) {
          strcpy(result, "NULL");
     } else if (count == 1) {
          strcpy(result, ids[0]);   // only one ID → return that
     } else {
          strcpy(result, ids[count - 2]); // return second last ID
     }

     return result;  // safe because it's static
}

void gettingConfigUserInfo(){
     char buff[256];

     const char * home = getenv("HOME");

     if (home == NULL) {
          forkCreationProblem();
          return ;
     }

     char path[1024];
     snprintf(path , sizeof(path) , "%s/NewGit2.0/configUser.txt" , home);

     FILE  * file = fopen(path , "r");

     if (file == NULL) {
          problemInCommit();
          return ;
     }

     spinnerAnimation2(CYN "Loading user info from config", 12, 70);

     while (fgets(buff , sizeof(buff) , file) != NULL) {
          printf( YEL "%s" END , buff );
     }

     fclose(file);
     return ;
}

void appendMessage(char * message) {

     printf(BLU "====================================\n" END);
     printf(BLU "        NewGit2.0 Commit Tool       \n" END);
     printf(BLU "====================================\n\n" END);

     spinnerAnimation2(CYN "Preparing commit", 15, 60);

     // Getting Id : 
     char * id = getingId();
     if (id == NULL){
          problemInCommit();
          return ;
     }
     else if (strcmp(id , "NULL") == 0) {
          NotStaged();
          return ;
     }

     // Counting the Numbers-----------------------------------
     spinnerAnimation2(CYN "Counting current directory files", 15, 60);
     struct FileCount currCount = countCurrentDir(); 

     spinnerAnimation2(CYN "Counting staged snapshot files", 15, 60);
     struct FileCount stagCount = countInStagDIR(id);

     if (currCount.files == -1 && currCount.folders == -1) {
          problemInCommit();
          return ;
     }
     if (stagCount.files == -1 && stagCount.folders == -1) {
          problemInCommit();
          return ;
     }

     //---------------------------------------------------------------------------
     const char * filePath = ".newgit/idInfo.txt";

     FILE * file = fopen(filePath , "a");
     if (file == NULL) {
          problemInCommit();
          return ;
     }

     printf(GRN "\nUser information ....\n" END);
     gettingConfigUserInfo();

     fprintf(file ,"Commit Message : %s\n" ,message );
     fclose(file);

     //-----------------Normal compare ------------------------
     printf(CYN "\nAnalyzing changes...\n" END);
     spinnerAnimation2("Comparing current state with staged snapshot", 20, 50);

     int isChnaged = 0;
     if (currCount.files < stagCount.files) {
          int totalCount = stagCount.files - currCount.files;
          printf("Files Contents : (Total count : %d)", totalCount);
          for (int i = 0; i < totalCount; i++){
               printf( RED "-" END );
          }
          printf("\n");
          isChnaged = 1;
     }
     if (currCount.folders < stagCount.folders) {
          int totalCount = stagCount.folders - currCount.folders; 
          printf("Folders Contents : (Total count : %d)", totalCount);
          for (int i = 0; i < totalCount; i++){
               printf( RED "-" END );
          }
          printf("\n");
          isChnaged = 1;
     }
     if (currCount.files > stagCount.files) {
          int totalCount = -(stagCount.files - currCount.files);
          printf("Files Contents : (Total count : %d)", totalCount);
          for (int i = 0; i < totalCount; i++){
               printf( GRN "+" END );
          }
          printf("\n");
          isChnaged = 1;
     }
     if (currCount.folders > stagCount.folders){
          int totalCount = -(stagCount.folders - currCount.folders);
          printf("Folder Contents : (Total count : %d)", totalCount);
          for (int i = 0; i < totalCount; i++){
               printf( GRN "+" END );
          }
          printf("\n");
          isChnaged = 1;
     }
     if (!isChnaged) {
          printf(GRN "Nothing Changed :) \n" END);
     }

     printf("\n\n");
     printf(YEL "Thank You for using NewGit2.0\n" END);
     printf(CYN "NewGit2.0 ---" VERSION "\n" END);
     return ;
}
