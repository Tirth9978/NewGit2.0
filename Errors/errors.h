#ifndef ERRORS_H
#define ERRORS_H

void forkCreationProblem();
void invalidCommand();

// user...
void userConfig() ;
void NotFoundUserName() ;
void SyntaxInvalid() ;

// File 
void fileCreationConfigError() ;
void writingConfigError();
void fileCreationErrorInDotGit() ;

// Init
void ProblemInInit();

// Staging 
void faultStaging();
#endif