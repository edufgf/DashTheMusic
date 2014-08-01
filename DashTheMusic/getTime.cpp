#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <string>
#include <vector>
#include <sys\timeb.h> 
using namespace std;


int main(){
   vector <float> vet;
   
   struct timeb start, end;
   int diff;
   unsigned int secs;
   
   getchar(); 
   printf("LENDO...\n");
   ftime(&start);  
   while(1){    
       getchar();
       ftime(&end);   
       secs=(end.time - start.time);
       diff = (int) (1000.0 * secs
        + (end.millitm - start.millitm));
       printf("Seconds: %u Miliseconds: %u\n", diff/1000+40,diff%1000);  
   } 
   return 0;
}
