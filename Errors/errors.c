#include<stdio.h>
#include "errors.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"

void invalidCommand() {
     printf(RED "Invalid Command . \n" END);
     printf(GRN "git <...> <....> <...> .. . . . " END);
     return ;
}

void forkCreationProblem() {
     printf(RED "Internal Problem Please tyr again Leter :(" END);
     return ;
}

void userConfig(char * str) {
     printf(RED "%s , Invalid\n" , str  , END);
     printf("  ^\n");
     printf("  |\n");
     printf(GRN "user.email\n" END);
     printf(GRN "user.name\n" END);
     return ;
}

void NotFoundUserName() {
     printf( RED "Please Enter the your Username :)\n" END );
     printf( RED "Correct | git config --global user.name <Username>" END );
     return ;
}