/**hi**/


#include "flagger.h"
#include <iostream>

using namespace std;

volatile bool flag = false;


string tlogname = "cars.log";
string flogname = "flagger.log";
void* criticalSection(void* args);

flagger flagger(&criticalSection, flogname, tlogname, 'n'); 


int main(int argc, char** argv){

  //cout << "Flag pre: " << flag<<"\n";
  
  
  flagger.make_t(1);
  
  flagger.create_t();

  flagger.join();
  
  //cout<< "Flag post: "<<flag<<"\n";
  
 return 1;
}
//make a mutex lock for this variable 
volatile int carCount(0);//<<<<<this one
//^^^^this needs a mutex lock^^^
//we could make a function call that sends the flagger class the address of 
//this vcariable and it makes a mutex lock for it


void* criticalSection(void* args){
  
  flag = true;
   cout<<"There is a car pulling up...\n";
   
  if(0==flagger.get_w()){
     carCount++;
     cout<<"Car number " << carCount <<"  pulling through..\n";
     flagger.sleep(2);
     cout<<"Car number " << carCount<<" is leaving(2 seconds later)\n";
     flagger.post();
  }else
    flagger.wait();
    carCount++;
    cout<<"Car number " << carCount <<"  pulling through..\n";
    flagger.sleep(2);
    cout<<"Car number " << carCount<<" is leaving(2 seconds later)\n";
    
  
  return NULL;
}

