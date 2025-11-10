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

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"


//Structures
struct FileCount{
     int files ; 
     int folders;
};

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

    fp = fopen(".newgit/idInfo.txt", "r");  // your file name
     if (fp == NULL) {
          problemInCommit();
          return 1;
     }

     while (fgets(line, sizeof(line), fp)) {
          // check if line starts with "id :"
          if (strncmp(line, "id :", 4) == 0) {
               char temp[100];
               // extract the id value after "id :"
               if (sscanf(line, "id : %s", temp) == 1 && strlen(temp) > 0) {
                    strcpy(ids[count++], temp);
               }
          }
     }

     fclose(fp);
     if (count == 0) {
          return "NULL";
     }
     else if (count == 1) {
          return ids[count - 1];
     }
     else {
          return ids[count - 2];
     }
}


void gettingConfigUserInfo(){
     char * userName ;
     char * userEmil ;
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

     while (fgets(buff , sizeof(buff) , file) != NULL) {
          printf( YEL "%s" END , buff );
     }
     return ;
}

void appendMessage(char * message) {

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
     printf("%s" , id);
     // Counting the Numbers-----------------------------------
     // struct FileCount currCount =countCurrentDir(); 
     // struct FileCount stagCount = countInStagDIR(id);
     // if (currCount.files == -1 && currCount.folders == -1) {
     //      problemInCommit();
     //      return ;
     // }
     // if (stagCount.files == -1 && stagCount.folders == -1) {
     //      problemInCommit();
     //      return ;
     // }


     //---------------------------------------------------------------------------
     // const char * filePath = ".newgit/idInfo.txt";

     // FILE * file = fopen(filePath , "a");
     // printf(GRN "User information ....\n" END);
     // gettingConfigUserInfo();
     // if (file == NULL) {
     //      problemInCommit();
     //      return ;
     // }

     // fprintf(file ,"Commit Message : %s\n" ,message );
     // fclose(file);

     //-----------------Normal compare ------------------------

     // if (currCount.files < stagCount.files) {
     //      int totalCount = stagCount.files - currCount.files;
     //      printf("Files Contents : (Total count : %d)" , totalCount);
     //      for (int i =0;i<totalCount;i++){
     //           printf( RED "-" END );
     //      }
     //      printf("\n");
     // }
     // if (currCount.folders < stagCount.folders) {
     //      int totalCount = stagCount.folders - currCount.folders ; 
     //      printf("Folders Contents : (Total count : %d)" , totalCount);
     //      for (int i =0;i<totalCount;i++){
     //           printf( RED "-" END );
     //      }
     //      printf("\n");
     // }
     
     return ;
}