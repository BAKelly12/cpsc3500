//lets do this right this time*/


#include <time.h>
#include <queue>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>

#define NUMBER_OF_CARS 50
#define MAX_WAITING_CARS 10

int pthread_sleep(int seconds);
void* criticalSection(void* args);
void* makeCar(void* args);


//global variable declarations
volatile int waiting;
volatile char direction = '-';
volatile int carCount(0);
volatile bool first_t = true;
volatile unsigned int wNorth(0);
volatile unsigned int wSouth(0);
volatile bool dSwitch = true; //True: North False: south


//semaphores and mutex
sem_t flag;
sem_t ques;

pthread_mutex_t quem;
pthread_mutex_t csmtx;//critical section mutex

/**
struct carg{
	char direction;
};

*/

using namespace std;


int main(){
	
	 sem_init(&flag, 0, 0);
	 pthread_mutex_init(&csmtx,NULL);
 
	 //make the critical section thread
	 srand(time(NULL));
	    pthread_t crit_s_thread;
		if ( -1 == pthread_create(&crit_s_thread, NULL, &criticalSection, NULL))
			{
				perror("error making critical section thread");
				return -1;
			} 
	
	 //make the north generator
	 
	 char* nArg = "N";	  
	 pthread_t makeNorth;
	 
	 if ( -1 == pthread_create(&makeNorth, NULL, &makeCar, (void*)nArg ))
		{
			perror("error making north car generator");
			return -1;
		} 
				
	 //make the south generator
	 
	 
	 char* sArg = "S";
	 pthread_t makeSouth;
	 if ( -1 == pthread_create(&makeSouth, NULL, &makeCar, &sArg )){
			perror("error making south car generator");
			return -1;
		}

		
	
	
	 pthread_join(crit_s_thread, NULL);
	 pthread_cancel(makeSouth);	
	 pthread_cancel(makeNorth);
	
	 
	 sem_destroy(&flag);
     pthread_mutex_destroy(&csmtx);
	 
	 cout<< wNorth << " " << wSouth <<endl;
	return 0;
}




void* criticalSection(void* args){
 
  while(carCount < NUMBER_OF_CARS){
	  
    if(first_t){
	   first_t = false; 
	   pthread_mutex_lock(&csmtx);
    }else{   
      sem_wait(&flag);  
      pthread_mutex_lock(&csmtx);  
    }//logic block for making threads wait 
	
     /////////START CRITICAL SECTION////////////////
	 
	 
	 
	 if(!wNorth && !wSouth){
		 //log sleeping
		cout<< "I SLEEP...\n";
		while(!(wNorth || wSouth)){}
		cout << "I HAVE AWOKEN...\n";
		if(wNorth)
			dSwitch = true;
		else
			dSwitch = false;
		}
	 else if( (dSwitch && wSouth >= MAX_WAITING_CARS) || !wNorth)
		 dSwitch = !dSwitch;
	 else if( (!dSwitch && wNorth >= MAX_WAITING_CARS) || !wSouth)
		 dSwitch = !dSwitch;
	 
	 
	 
	
	char myDir;
	if(dSwitch)//if current traveling direction is north
	{	
		myDir = 'N';
	}
	else{
		myDir = 'S';
	}
	 
	 
	if(myDir == 'N' && wNorth !=0)
		wNorth--;
	else if(myDir == 'S' && wSouth !=0)
		wSouth--;
	
    //cars that exist and are going through//
    
	
    int myCarCount = carCount;
	carCount++;	
    cout<<"Car number " << myCarCount <<" pulling through..DIR: "<<myDir<<"\n";
    pthread_sleep(2);
    cout<<"Car number " << myCarCount<<" is leaving(3 seconds later)\n";
 
   pthread_mutex_unlock(&csmtx);
   sem_post(&flag);  
   cout << wNorth << " " << wSouth <<"\n";   
  }
  return NULL;
} 



int pthread_sleep(int seconds){    
    pthread_mutex_t mutex;      
	pthread_cond_t conditionvar; 
	struct timespec timetoexpire;
	if(pthread_mutex_init(&mutex,NULL))
		{               return -1;
	}
	if(pthread_cond_init(&conditionvar,NULL))
		{               return -1;        
	}//When to expire is an absolute time, so get the current time and add 
	//it to our delay time 
	timetoexpire.tv_sec = (unsigned int)time(NULL) + seconds;
	timetoexpire.tv_nsec = 0;
	return pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
} 



void* makeCar(void* args){
	int i(0);
	while(1){
		
		pthread_sleep(2);//just to prevent hundreds of thousands of cars
    	char* direction;
    	direction = (char*)args;

    	  srand(time(NULL) + i);
    	  if(8 >= rand() % 10 + 1){ 
    	    	if(*direction == 'N')
    	    		++wNorth;
    	    	else
    	    		++wSouth;
    	  }else
    		pthread_sleep(20);
	
	    i++;	
	}
	
	return NULL;
}





