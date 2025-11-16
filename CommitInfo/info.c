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

     spinnerAnimation(CYN "Loading history", 15, 60);

     printf(GRN "\nHistory : \n\n" END );

     while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
          // Print original line (ID, message, etc.)
          printf(WHT "%s" END, buffer);  

          char *prefix = "Id:";
          char *pos = strstr(buffer, prefix);
          struct FileCount count = {-1,-1};

          if (pos != NULL) {
               char *id = pos + strlen(prefix);

               // Trim newline at end
               char *newline = strchr(id, '\n');
               if (newline) *newline = '\0';

               // Cool animation while counting files/folders for this ID
               spinnerAnimation(CYN "Scanning snapshot files", 12, 70);

               count = countInStagDIR_1(id);
          }

          printf(GRN "File Count   : %d\n" END, count.files);
          printf(GRN "Folder Count : %d\n" END, count.folders);

          printf(YEL "-------------------------------------------------\n\n" END);
     }

     fclose(file);

     printf(GRN "To get the full information in txt file : \n" END );
     printf(WHT "Type this command : open .newgit/idInfo.txt\n\n\n" END);

     printf(YEL "Thank You for using NewGit2.0\n" END);
     printf(CYN "NewGit2.0 ---" VERSION "\n" END);
     return ;
}
