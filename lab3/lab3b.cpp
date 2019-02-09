/**hi**/


#include "flagger.h"
#include <iostream>

using namespace std;


const void* criticalSection(void* args){
  return NULL;
}


int main(int argc, char** argv){
  
  string logname = "cars.log";
  
  flagger flagger(&criticalSection, logname);  
  

 return 1;
}