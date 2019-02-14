/**hi**/

#include <cstdlib>
#include <iostream>
#include <queue>
#include <cstdio>
#include <ctime>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

int pthread_sleep (int seconds);
void* makeCarsN(void* args);
void* makeCarsS(void* args);




//Need to make the road and the cars. This is a queue.
//Will need an "odd" or "randnum".
void* criticalSection(void* args);
void* makeCarsN(void* args);
void* makeCarsS(void* args);

pthread_mutex_t northVar, southVar;
const int numInteractions = 10;
int burstCarsN = 0;
int burstCarsS = 0;
int numCarsThru = 0;

//sem_t createStuff, releaseStuff;

int main()
{
	
	pthread_t makeCN, makeCS, critThread;
	
	pthread_create(&makeCN, NULL, makeCarsN, NULL);
	pthread_create(&makeCS, NULL, makeCarsS, NULL);
	pthread_create(&critThread, NULL, criticalSection, NULL);
	
	pthread_join(makeCN, NULL);
	pthread_join(makeCS, NULL);
	pthread_join(critThread, NULL);

	//sem_destroy(&createStuff);
	//sem_destroy(&releaseStuff);
}

//Critical part of the process.
void* criticalSection(void* args)
{
	while (numCarsThru != numInteractions)
	{
		pthread_mutex_lock(&northVar);
		pthread_mutex_lock(&southVar);
		
		if (burstCarsN == 0 && burstCarsS == 0)
			cout << "flagger is asleep";
		else if (burstCarsN > 0 && burstCarsS < 10){
			burstCarsN--;
			cout << "Car north goes thru" << endl;
		}
		else if (burstCarsS > 0 && burstCarsN < 10){
			burstCarsS--;
			cout << "Car south goes thru" << endl;
		}
		else {
			cout << "More than 10 or something went wrong.. following amount:";
			cout << endl << "CarsN: " << burstCarsN << endl;
			cout << "CarsS: " << burstCarsS << endl;
		}
		
		numCarsThru++;
		pthread_sleep(1);
	}
	return NULL;
}


//Create cars going north
void* makeCarsN(void* args)
{
	while (numCarsThru != numInteractions)
	{
		while (rand() % 10 + 1 <= 8)
		{
			pthread_mutex_lock(&northVar);
			burstCarsN++;
			pthread_mutex_unlock(&northVar);
		}
		pthread_sleep(20);
	}

	return NULL;
}

//Create cars going south
void* makeCarsS(void* args)
{
	while (numCarsThru != numInteractions)
	{
		while (rand() % 10 + 1 <= 8)
		{
			pthread_mutex_lock(&southVar);
			burstCarsS++;
			pthread_mutex_unlock(&southVar);
		}
		pthread_sleep(20);
	}
	
	return NULL;
}



int pthread_sleep (int seconds)
{        
	pthread_mutex_t mutex;
	pthread_cond_t conditionvar;
	struct timespec timetoexpire;       

	if(pthread_mutex_init(&mutex,NULL))       
	{               
		return -1;        
	}        
	
	if(pthread_cond_init(&conditionvar,NULL))       
	{               
		return -1;        
	}        
	//When to expire is an absolute time, so get the current time and add
	//it to our delay time
	timetoexpire.tv_sec = (unsigned int)time(NULL) + seconds;
  timetoexpire.tv_nsec = 0; 
  return pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
} 
