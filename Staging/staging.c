#include <stdio.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>
#include <string.h>
#include "staging.h"

const char * generateId() {
     const char * data1 = "ASDFGHJKLZXCVBNMPOIUYTREWQ";
     const char * data2 = "1234567890";
     const char * data3 = "qwertyuioplkjhgfdsazxcvbnm";
     const char * data4 = "!@#$&*";

     static char ans[100];
     ans[0] = '\0';  // clear buffer

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
