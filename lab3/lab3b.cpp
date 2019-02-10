/**hi**/


#include "flagger.h"
#include <iostream>

using namespace std;

bool flag = false;


string tlogname = "cars.log";
string flogname = "flagger.log";
void* criticalSection(void* args);

flagger flagger(&criticalSection, flogname, tlogname, 'n'); 


int main(int argc, char** argv){

  cout << "Flag pre: " << flag<<"\n";
  
  
  flagger.make_t(1);
  flagger.sleep(2);
  flagger.join();
  
  cout<< "Flag post: "<<flag<<"\n";
  
 return 1;
}

void* criticalSection(void* args){
  
  flag = true;
  cout<<"hello\n";
  
  return NULL;
}

