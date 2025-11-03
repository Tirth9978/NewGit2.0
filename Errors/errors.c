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

#define VERSION "1.0.1"

void invalidCommand() {
     printf(RED "Invalid Command . \n" END);
     printf(GRN "git <...> <....> <...> .. . . . \n" END);
     return ;
}

void forkCreationProblem() {
     printf(RED "Internal Problem Please try again Leter :(" END);
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
     printf( RED "Correct | git config --global user.name <Username>\n" END );
     return ;
}

void SyntaxInvalid() {
     printf( RED "Invalid Command\n" END );
     return ;
}

// -------------------------------------------- File --------------------------------------------//
void fileCreationConfigError()  {
     printf(RED "Here , There is a error in the file creation , Please Try again leter :(" END);
     return ;
}
void writingConfigError() {
     printf(RED "Sorry , There is problem to config user :(\n" END);
     return ;
}
void fileCreationErrorInDotGit() {
     printf( RED "There is the problem in initialization of the git repo :( , Please try again leter \n" END );
     return ;
}



// -------------------------------------------- Staging --------------------------------------------//

void faultStaging() {
     printf(RED "There is a problem with the staging :(, Please Try Again Leter ....\n" END);
     return ;
}

// -------------------------------------------- Init --------------------------------------------//


void ProblemInInit() {
     printf( RED "Problem found in initialization\n" END );
     printf( RED "Try Again Leter :(\n" END );
     return ;
}

// -------------------------------------------- Init --------------------------------------------//
void ForGotMessage() {
     printf( RED "You forget the message . \n" END );
     printf( YEL "Please add the commit message\n" END );
     return ;
}

void Forgot_M() {
     invalidCommand() ;
     printf( GRN "Correct :\n" END );
     printf( GRN "newgit commit -m <Message>\n" END );
     return ;
}

void problemInCommit(){
     printf(RED "Problem in commiting the message :(\n" END);
     printf(RED "Please Try again leter ...\n" END);
     return ;
}