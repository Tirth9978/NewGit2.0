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

#define BUFFER_SIZE 8192  // 8KB buffer


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
          fprintf(stderr, "Error: Invalid ID or directory not found at %s\n", srcPath);
          return;
     }

     printf("Reverting from %s → current directory...\n", srcPath);
     copyDirectory(srcPath, ".");
     printf("Revert completed successfully.\n");
}

// Recursive directory copy (skipping .newgit)
void copyDirectory(const char *srcDir, const char *destDir) {
    DIR *dir = opendir(srcDir);
    if (!dir) return;

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
                // Recursively copy
                copyDirectory(srcPath, destPath);
            } else if (S_ISREG(st.st_mode)) {
                // Copy regular file
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
        fprintf(stderr, "Error opening source file: %s\n", srcFile);
        return;
    }

    FILE *dest = fopen(destFile, "wb");
    if (!dest) {
        fprintf(stderr, "Error creating destination file: %s\n", destFile);
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
     if (!exist(id)) {
          idNotExist();
          return ;
     }

     newgit_revert(id);
}