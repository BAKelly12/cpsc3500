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
  
  
  flagger.make_t(2);
  
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
   //flagger.sleep();
   cout<<"There is a car pulling up...\n";
   
  if(flagger.first_t()){
    //Get mutex for carCount
     flagger.get_lock();   
  }else{   
    flagger.wait();  
    flagger.get_lock();
  }
   //get mutex for carCount
    carCount++;
    int myCarCount = carCount; 
    cout<<"Car number " << myCarCount <<"  pulling through..\n";
    flagger.sleep(2);
    cout<<"Car number " << myCarCount<<" is leaving(3 seconds later)\n";
    //release mutex for carCount
    flagger.post();
  return NULL;
}
//after the thrads are all clear there should be a signal to reset the first thread//
//bool inside the class..  that way it can be reused repeatedly

