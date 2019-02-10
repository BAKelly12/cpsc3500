/**hi**/


#include "flagger.h"
#include <iostream>

using namespace std;

bool flag = false;

void* criticalSection(void* args){
  
  flag = true;
  cout<<"hello\n";
  
  return NULL;
}


int main(int argc, char** argv){
  
  string logname = "cars.log";
  
  flagger flagger(&criticalSection, logname);  

  cout << "Flag pre: " << flag<<"\n";
  
  flagger.consume();
  
  cout<< "Flag post: "<<flag<<"\n";
  
 return 1;
}


