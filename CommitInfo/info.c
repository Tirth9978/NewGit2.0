#include <stdio.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "info.h"
#include "../Errors/errors.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"

#define BUFFER_SIZE 256 // Choose an appropriate buffer size
#define VERSION "1.0.1"

struct FileCount{
     int files ; 
     int folders;
};

// Spinner animation (same style as previous code)
void spinnerAnimation1(const char *message, int cycles, int delayMs) {
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

struct FileCount countFilesInPath_1(const char *basePath) {
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
                struct FileCount subCount = countFilesInPath_1(fullPath);
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

struct FileCount countInStagDIR_1(char * id ) {
     char path[256];
     snprintf(path, sizeof(path), ".newgit/StagingInfo/%s", id);
     return countFilesInPath_1(path);
}

void gettingInfo(){
     FILE * file = fopen(".newgit/idInfo.txt" , "r");
     char buffer[BUFFER_SIZE];
     if (file == NULL) {
          forkCreationProblem();
          return ;
     }

     printf(BLU "======================================\n" END);
     printf(BLU "        NewGit2.0 Commit History      \n" END);
     printf(BLU "======================================\n\n" END);

     spinnerAnimation1(CYN "Loading history", 15, 60);

     printf(GRN "\nHistory : \n\n" END );

     while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
          // Print original line (ID, message, etc.)
          printf(WHT "%s" END, buffer);  

          char *prefix = "Id:";
          char *pos = strstr(buffer, prefix);
          struct FileCount count = {-1,-1};

          if (pos != NULL ) {
               char *id = pos + strlen(prefix);

               // Trim newline at end
               char *newline = strchr(id, '\n');
               if (newline) *newline = '\0';

               // Cool animation while counting files/folders for this ID
               spinnerAnimation1(CYN "Scanning snapshot files", 12, 70);

               count = countInStagDIR_1(id);
          }    
          if (count.files != -1 && count.folders != -1) {
               printf(GRN "File Count   : %d\n" END, count.files);
               printf(GRN "Folder Count : %d\n" END, count.folders);
          }
          

          
     }

     fclose(file);

     printf(GRN "To get the full information in txt file : \n" END );
     printf(WHT "Type this command : open .newgit/idInfo.txt\n\n\n" END);

     printf(YEL "Thank You for using NewGit2.0\n" END);
     printf(CYN "NewGit2.0 ---" VERSION "\n" END);
     return ;
}

int empty_directory(char *path) {
    DIR *dir;
    struct dirent *entry;
    char full_path[1024]; // Buffer for full path

    if (!(dir = opendir(path))) {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(full_path, &st) == -1) {
            perror("lstat");
            closedir(dir);
            return -1;
        }

        if (S_ISDIR(st.st_mode)) {
            // Recursively empty subdirectory
            if (empty_directory(full_path) == -1) {
                closedir(dir);
                return -1;
            }
            // Remove empty subdirectory
            if (rmdir(full_path) == -1) {
                perror("rmdir");
                closedir(dir);
                return -1;
            }
        } else {
            // Delete file
            if (unlink(full_path) == -1) {
                perror("unlink");
                closedir(dir);
                return -1;
            }
        }
    }

    closedir(dir);
    return 0;
}

void removeHistory(){

     char ch ;
     printf("Are you sure you want to remove the History (y or n): ");
     scanf("%c" , &ch);
     if (ch == 'n') {
          return ;
     }
     else if (ch == 'y') {
          FILE * file = fopen(".newgit/idInfo.txt" , "w");
          char buffer[BUFFER_SIZE];
          if (file == NULL) {
               forkCreationProblem();
               return ;
          }

          fclose(file);
          int ref= empty_directory(".newgit/StagingInfo/");
          printf(GRN "Your History successfully removed ...\n" END);
     }
     else {
          printf(END "Please enter the Valid things :(\n" END);
     }
     
     return ;
}