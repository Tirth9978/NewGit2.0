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

struct FileCount countInStagDIR(char * id){
     printf("%s\n" , id);
     struct FileCount count= {-1,-1};
     char path[200];
     snprintf(path, sizeof(path), ".newgit/StagingInfo/%s", id);
     struct dirent *dp;
     struct stat st;
     DIR *dir = opendir(path);


     if (!dir)
        return count; 
     
     count.files = 0 ;
     count.folders = 0;


     while ((dp = readdir(dir)) != NULL) {
          // Ignore . and ..
          if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
               continue;

          char path[1024];
          snprintf(path, sizeof(path), "%s/%s", path, dp->d_name);

          if (stat(path, &st) == 0) {
               if (S_ISDIR(st.st_mode)) {
                    count.folders++;
                    struct FileCount subCount = countInStagDIR(path);
                    count.files += subCount.files;
                    count.folders += subCount.folders;
               } else if (S_ISREG(st.st_mode)) {
                    count.files++;
               }
          }
     }
     closedir(dir);
     return count ;
}


char *getingId() {
     const char *path = ".newgit/idInfo.txt";
     FILE *file = fopen(path, "r");   // open for reading
     if (file == NULL) {
          return "NULL";
     }

     static char id[256];
     char buffer[256];

     while (fgets(buffer, sizeof(buffer), file) != NULL) {
          // Look for line starting with "Id:"
          if (strncmp(buffer, "Id:", 3) == 0) {
               strcpy(id, buffer + 3);   // copy text after "Id:"
               // remove newline if any
               id[strcspn(id, "\n")] = '\0';
          }
     }

     fclose(file);
     return id;
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
     printf("I am in the USer\n");
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
     // Counting the Numbers-----------------------------------
     struct FileCount currCount =countCurrentDir(); 
     struct FileCount stagCount = countInStagDIR(id);
     printf("Stag : %d %d\n" , stagCount.files , stagCount.folders);
     if (currCount.files == -1 && currCount.folders == -1) {
          problemInCommit();
          return ;
     }


     //---------------------------------------------------------------------------
     const char * filePath = ".newgit/idInfo.txt";

     FILE * file = fopen(filePath , "a");
     printf(GRN "User information ....\n" END);
     gettingConfigUserInfo();
     if (file == NULL) {
          problemInCommit();
          return ;
     }

     fprintf(file ,"Commit Message : %s\n" ,message );
     fclose(file);

     

     return ;
}