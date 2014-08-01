#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <string>
#include <vector>
#include <sys\timeb.h> 
using namespace std;


int main(){
   double time,pos;
   char buff[1000];
   int k=0;
   while(gets(buff)!=NULL){
       sscanf(buff,"%Lf %Lf",&time,&pos);
       if (time>=2.2){
       printf("note[%d].time=%lf;\n",k,time);
       printf("note[%d].time=%lf;\n",k+1,time);
       printf("build_note2(&note[%d],&note[%d],disp_data.width,%lf-2*ball.rad,5*ball.rad,30,20,\"wood1.png\");\n",k,k+1,pos);           
       k+=2;
       }
       int cnt=0;
       while(cnt<49){
            gets(buff);  
            cnt++;                   
       }        
   }
   
   
   return 0;
}
