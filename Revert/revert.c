#include <stdio.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "revert.h"
#include "../Errors/errors.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"

#define BUFFER_SIZE 8192  // 8KB buffer
#define VERSION "1.0.1"

// =============== Spinner Animation 4 ===============
void spinnerAnimation4(const char *message, int cycles, int delayMs) {
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

// Function declarations
void copyDirectory(const char *srcDir, const char *destDir);
void copyFile(const char *srcFile, const char *destFile);

int exist(char * id){
     const char *path = ".newgit/idInfo.txt";
     FILE *fp = fopen(path, "r");
     char line[200];
     char temp[100];

     if (fp == NULL) {
          problemInRevert();
          return 0;
     }
     while (fgets(line, sizeof(line), fp)) {
          // Check if line starts with "Id:"
          if (strncmp(line, "Id:", 3) == 0) {
               if (sscanf(line, "Id:%s", temp) == 1 && strlen(temp) > 0) {
                    // Compare extracted ID with the given one
                    if (strcmp(temp, id) == 0) {
                         fclose(fp);
                         return 1;  // found the ID
                    }
               }
         
          }
     }
     fclose(fp);
     return 0;  // not found
}

void newgit_revert(const char *id) {
     char srcPath[512];
     snprintf(srcPath, sizeof(srcPath), ".newgit/StagingInfo/%s", id);

     struct stat st;
     if (stat(srcPath, &st) != 0 || !S_ISDIR(st.st_mode)) {
          fprintf(stderr, RED "Error: Invalid ID or directory not found at %s\n" END, srcPath);
          return;
     }

     printf(BLU "====================================\n" END);
     printf(BLU "        NewGit2.0 Revert Tool       \n" END);
     printf(BLU "====================================\n\n" END);

     printf(CYN "Target snapshot ID : " WHT "%s\n" END, id);
     printf(CYN "Snapshot location  : " WHT "%s\n\n" END, srcPath);

     spinnerAnimation4(CYN "Preparing to restore files", 18, 60);

     printf(YEL "Reverting from snapshot → current directory...\n" END);
     printf(WHT "(Existing files with same names will be overwritten)\n\n" END);

     spinnerAnimation4(CYN "Copying snapshot contents", 20, 50);

     copyDirectory(srcPath, ".");

     printf("\n");
     printf(GRN "Revert completed successfully.\n" END);
}

// Recursive directory copy (skipping .newgit)
void copyDirectory(const char *srcDir, const char *destDir) {
    DIR *dir = opendir(srcDir);
    if (!dir) {
        fprintf(stderr, RED "Error: Could not open directory %s\n" END, srcDir);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        // Skip .newgit folder
        if (strcmp(entry->d_name, ".newgit") == 0)
            continue;

        char srcPath[1024], destPath[1024];
        snprintf(srcPath, sizeof(srcPath), "%s/%s", srcDir, entry->d_name);
        snprintf(destPath, sizeof(destPath), "%s/%s", destDir, entry->d_name);

        struct stat st;
        if (stat(srcPath, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                // Create directory if not exists
                mkdir(destPath, 0755);
                printf(CYN "Created directory: " WHT "%s\n" END, destPath);
                // Recursively copy
                copyDirectory(srcPath, destPath);
            } else if (S_ISREG(st.st_mode)) {
                printf(CYN "Restoring file   : " WHT "%s\n" END, destPath);
                spinnerAnimation4("   Writing data", 8, 40);
                copyFile(srcPath, destPath);
            }
        }
    }
    closedir(dir);
}

// Buffered file copy function
void copyFile(const char *srcFile, const char *destFile) {
    FILE *src = fopen(srcFile, "rb");
    if (!src) {
        fprintf(stderr, RED "Error opening source file: %s\n" END, srcFile);
        return;
    }

    FILE *dest = fopen(destFile, "wb");
    if (!dest) {
        fprintf(stderr, RED "Error creating destination file: %s\n" END, destFile);
        fclose(src);
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes;
    while ((bytes = fread(buffer, 1, BUFFER_SIZE, src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);
}

void revertById(char * id) {
     printf(BLU "====================================\n" END);
     printf(BLU "        NewGit2.0 Revert Tool       \n" END);
     printf(BLU "====================================\n\n" END);

     spinnerAnimation4(CYN "Validating snapshot ID", 15, 60);

     if (!exist(id)) {
          idNotExist();
          return ;
     }

     printf(GRN "Snapshot ID %s is valid.\n\n" END, id);

     newgit_revert(id);

     printf("\n\n");
     printf(YEL "Thank You for using NewGit2.0\n" END);
     printf(CYN "NewGit2.0 ---" VERSION "\n" END);
}
