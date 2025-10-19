#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MakeConfig.h"


#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"

int findMainConfigFile (){
     const char *homeDir = getenv("HOME");

     if (!homeDir){
          printf(RED "Error: HOME environment variable not set.\n" END) ;
          return 1;
     }

     char * filePath = malloc(strlen(homeDir) + strlen("/NewGitConfig") + 1);

     if (!filePath){
          printf(RED "Memory allocation failed.\n" END);
          return 2;
     }
     sprintf(filePath, "%s/NewGitConfig", homeDir);

     FILE * file = fopen(filePath , "r");

     if (file){
          
          fclose(file);
          free(filePath);
          return 1;
     }
     else {
          free(filePath);
          return 0;
     }
}