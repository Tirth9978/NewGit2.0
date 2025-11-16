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

void gettingInfo(){
     FILE * file = fopen(".newgit/idInfo.txt" , "r");
     char buffer[BUFFER_SIZE];
     if (file == NULL) {
          forkCreationProblem();
          return ;
     }

     printf( GRN "History : \n" END );

     while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
          // Process the line stored in 'buffer'
          printf("%s\n", buffer);
     }
     
     return ;
}