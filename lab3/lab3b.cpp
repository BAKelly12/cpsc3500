/**hi**/


#include "flagger.h"
#include <iostream>
#include <cstdlib>
#include <queue>

#define NUMBER_OF_THREADS 100
#define NUMBER_OF_ROTATIONS 4

using namespace std;

volatile bool flag = false;


string tlogname = "cars.log";
string flogname = "flagPerson.log";
void* criticalSection(void* args);
void* getDirection();


flagger flagPerson(&criticalSection, flogname, tlogname); 
flagger dirMtx;

int main(int argc, char** argv){ 

  srand(time(NULL));
  flagPerson.make_t((int)NUMBER_OF_THREADS);
  
  flagPerson.create_t();

  flagPerson.join();
 
 return 1;
}


volatile int carCount(0);
queue<char> carQueue;
volatile char direction = '-';
volatile int opCount(0);


void* criticalSection(void* args){
  srand((int)*args);
    
  if(dirMtx.first_t())
    dirMtx.get_lock();   
  else{   
    dirMtx.wait();  
    dirMtx.get_lock(); 
  }

   getDirection();
   if(myPreDir = 'N'){
     carQueue.push(direction);   
   else if(myPreDir = 'S')

	   
  dirMtx.post();
  dirMtx.release_lock();
  
  
  while (carCount.front() != '-')
  {
    if (
       tempCarQueue.push(carQueue.front());
       if (carQueue.front() !=
         opCount++;
       carQueue.pop();
    }
  }
  
    
  if (opCount >= 10)
    sem_wait(&carOpposite);


  if(flagPerson.first_t()){
     flagPerson.get_lock();   
  }else{   
    flagPerson.wait();  
    flagPerson.get_lock();
   
  }//logic block for making threads wait
  
     /////////START CRITICAL SECTION////////////////
   
   char myDir = carQueue.front();
   carQueue.pop();
 
 
  if(myDir == '-'){
    cerr<<"no cars coming.. I SLEEP...\n";
    direction = 'N';
    flagPerson.sleep(2);
    cerr<<"I'm done sleeping\n";
  }
  else{
   
    carCount++;
    int myCarCount = carCount; 
    cout<<"Car number " << myCarCount <<" pulling through..DIR: "<<myDir<<"\n";
    flagPerson.sleep(1);
    cout<<"Car number " << myCarCount<<" is leaving(3 seconds later)\n";
  }

   
   flagPerson.release_lock();
   flagPerson.post();
 
  return NULL;
} 





/*
void* getDirection(){ 
  
   //srand(time(arg->t));

  
  if(direction == '-'){//if no cars coming, generate a 50-50 chance of where a car is
    if(rand() % 10 + 1 <5)
      direction = 'N';
    else
      direction = 'S';
  }
  
  if(rand() % 10 + 1 <=9)//if there's a car at all
  {
    if (8 <= rand() % 10 + 1)//is it the same direction?   
    { //Change direction
      if (direction == 'N')
      {
        direction = 'S';
      }
      else if(direction == 'S')
      {
         direction = 'N';
      }
    }
  }else//no car
  {
    direction = '-';
  }
 

 return NULL;
}
*/