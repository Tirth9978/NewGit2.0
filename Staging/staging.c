#include <stdio.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "staging.h"
#include "../Errors/errors.h"

#define BUFFER_SIZE 4096

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"

#define VERSION "1.0.1"

// ===================== Spinner Animation 5 =====================
void spinnerAnimation5(const char *message, int cycles, int delayMs) {
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

const char * generateId() {
    const char * data1 = "ASDFGHJKLZXCVBNMPOIUYTREWQ";
    const char * data2 = "1234567890";
    const char * data3 = "qwertyuioplkjhgfdsazxcvbnm";
    const char * data4 = "!@#$&*";

    static char ans[100];
    ans[0] = '\0';

    // Seed rand only once
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }

    for (int count = 0; count < 3; count++) {
        char temp[2];
        temp[1] = '\0';

        temp[0] = data1[rand() % 26];
        strcat(ans, temp);

        temp[0] = data2[rand() % 10];
        strcat(ans, temp);

        temp[0] = data3[rand() % 26];
        strcat(ans, temp);

        temp[0] = data4[rand() % 6];
        strcat(ans, temp);
    }

    return ans;
}

char* getDateTime() {
    static char buffer[100]; // static so it persists after function ends
    time_t t;
    struct tm* timeInfo;

    time(&t);                 // get current time
    timeInfo = localtime(&t); // convert to local time format

    // Format: DD-MM-YYYY HH:MM:SS
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeInfo);

    return buffer;
}

void copy_file(const char *src_path, const char *dest_path) {
    FILE *src = fopen(src_path, "rb");
    if (!src) {
        faultStaging();
        return;
    }

    FILE *dest = fopen(dest_path, "wb");
    if (!dest) {
        faultStaging();
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

// ✅ Recursive function to copy directories and files
void copy_recursive(const char *src_dir, const char *dest_dir) {
     DIR *dir = opendir(src_dir);
     if (!dir) return;

     mkdir(dest_dir, 0777);

     struct dirent *entry;
     while ((entry = readdir(dir)) != NULL) {
          // Skip . and ..
          if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
               continue;

          // Skip .newgit folder itself to prevent recursion into staging area
          if (strcmp(entry->d_name, ".newgit") == 0)
               continue;

          char src_path[512];
          char dest_path[512];

          snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
          snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, entry->d_name);

          struct stat st;
          stat(src_path, &st);

          if (S_ISDIR(st.st_mode)) {
               printf(CYN "Adding folder to staging : " WHT "%s\n" END, dest_path);
               spinnerAnimation5("   Copying folder", 8, 40);
               // If directory → recurse
               copy_recursive(src_path, dest_path);
          } else {
               printf(CYN "Adding file to staging   : " WHT "%s\n" END, dest_path);
               spinnerAnimation5("   Copying file", 8, 40);
               // If file → copy
               copy_file(src_path, dest_path);
          }
     }

    closedir(dir);
}

// ✅ Main function to handle staging copy
void movingFilesToStaheFolder(char *basePath, char *id) {
     char final_path[512];
     snprintf(final_path, sizeof(final_path), "%s%s", basePath, id);

     printf(BLU "====================================\n" END);
     printf(BLU "        NewGit2.0 Staging Tool      \n" END);
     printf(BLU "====================================\n\n" END);

     printf(CYN "Staging ID         : " WHT "%s\n" END, id);
     printf(CYN "Staging location   : " WHT "%s\n\n" END, final_path);

     spinnerAnimation5(CYN "Preparing staging directory", 15, 60);

     mkdir(final_path, 0777);

     printf(YEL "Copying current project into staging...\n\n" END);
     spinnerAnimation5(CYN "Copying project files", 20, 50);

     // Copy everything recursively from current folder
     copy_recursive(".", final_path);

     printf("\n");
     printf(GRN "✅ Repo successfully added to Staging Environment!\n" END);
     printf(CYN "NewGit2.0 --->" VERSION "\n" END);
}

// ✅ Add staging info and perform copy
void addingStaging() {
     printf(BLU "====================================\n" END);
     printf(BLU "        NewGit2.0 Staging Tool      \n" END);
     printf(BLU "====================================\n\n" END);

     spinnerAnimation5(CYN "Generating staging ID", 12, 70);
     const char *id = generateId();

     const char *filePath = ".newgit/idInfo.txt";

     FILE *file = fopen(filePath, "a");
     if (file == NULL) {
          faultStaging();
          return;
     }

     char *dateTime = getDateTime();

     spinnerAnimation5(CYN "Writing staging metadata", 12, 70);

     fprintf(file, "Id:%s\n", id);
     fprintf(file, "Day/Time: %s\n", dateTime);
     fclose(file);
    
     movingFilesToStaheFolder(".newgit/StagingInfo/", (char *)id);
}
