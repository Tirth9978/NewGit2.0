#ifndef STAGING_H
#define STAGING_H

const char * generateId() ;
void addingStaging() ;
void movingFilesToStaheFolder(char * filepath , char* id) ;
char * getDateTime();
void copy_file(const char *src_path, const char *dest_path);


#endif