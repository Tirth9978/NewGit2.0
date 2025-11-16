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

struct FileCount{
     int files ; 
     int folders;
};


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


void gettingInfo(){
     FILE * file = fopen(".newgit/idInfo.txt" , "r");
     char buffer[BUFFER_SIZE];
     if (file == NULL) {
          forkCreationProblem();
          return ;
     }

     printf( GRN "History : \n" END );

     while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
          printf("%s", buffer); 

          char *prefix = "Id:";
          char *pos = strstr(buffer, prefix);
          if (pos != NULL) {
               char *id = pos + strlen(prefix);

               char *newline = strchr(id, '\n');
               if (newline) *newline = '\0';

               printf("Extracted ID: %s\n", id);
          }
     }
     printf(GRN "To get the full information in txt file : \n" END );
     printf("Type this command : open .newgit/idInfo.txt\n");
     return ;
}