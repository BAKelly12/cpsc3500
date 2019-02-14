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
const int numInteractions = 200;
int burstCarsN = 0;
int burstCarsS = 0;
int numCarsThru = 0;

sem_t northStuff, southStuff;

int main()
{
	
	sem_init(&northStuff, 0, 0);
	sem_init(&southStuff, 0, 0);
	
	pthread_t makeCN, makeCS, critThread;
	
	pthread_create(&makeCN, NULL, makeCarsN, NULL);
	pthread_create(&makeCS, NULL, makeCarsS, NULL);
	pthread_create(&critThread, NULL, criticalSection, NULL);
	
	pthread_join(makeCN, NULL);
	pthread_join(makeCS, NULL);
	pthread_join(critThread, NULL);

	sem_destroy(&northStuff);
	sem_destroy(&southStuff);
}

//Critical part of the process.
void* criticalSection(void* args)
{
	while (numCarsThru != numInteractions)
	{
		sem_wait(&northStuff);
		sem_wait(&southStuff);
			
		if (burstCarsN == 0 && burstCarsS == 0){
			cout << "flagger is asleep, number: " << numCarsThru << endl;
			pthread_sleep(1);
			numCarsThru++;
		}
		else if (burstCarsN > 0 && burstCarsS < 10){
			while (burstCarsN > 0 && burstCarsS < 10){
				burstCarsN--;
				cout << "Car north goes thru number: " << numCarsThru << endl;
				pthread_sleep(1);
				numCarsThru++;
			}
		}
		else if (burstCarsS > 0 && burstCarsN < 10){
			while (burstCarsS > 0 && burstCarsN < 10){
				burstCarsS--;
				cout << "Car south goes thru number: " << numCarsThru << endl;
				pthread_sleep(1);
				numCarsThru++;
			}
		}
		else {
			cout << "More than 10 or something went wrong.. following amount:";
			cout << endl << "CarsN: " << burstCarsN << endl;
			cout << "CarsS: " << burstCarsS << endl;
			pthread_sleep(1);
			numCarsThru++;
		
		}

		sem_post(&northStuff);
		sem_post(&southStuff);


	}
	return NULL;
}


//Create cars going north
void* makeCarsN(void* args)
{
	while (numCarsThru != numInteractions)
	{
		//First car always comes
		pthread_mutex_lock(&northVar);
		burstCarsN++;
		pthread_mutex_unlock(&northVar);
		
		while (rand() % 10 + 1 <= 8) //Do the 80%
		{
			pthread_mutex_lock(&northVar);
			burstCarsN++;
			pthread_mutex_unlock(&northVar);
		}
		for (int i = 1; i < 11; i++)
		{
			sem_post(&northStuff);
			pthread_sleep(1);
			//cout << "north asleep for " << i << "seconds" << endl;
			sem_wait(&northStuff);
		}
	}

	return NULL;
}

//Create cars going south
void* makeCarsS(void* args)
{
	while (numCarsThru != numInteractions)
	{
		//First car always comes
		pthread_mutex_lock(&southVar);
		burstCarsS++;
		pthread_mutex_unlock(&southVar);
		
		while (rand() % 10 + 1 <= 8) //Do the 80%
		{
			pthread_mutex_lock(&southVar);
			burstCarsS++;
			pthread_mutex_unlock(&southVar);
		}
		for (int i = 1; i < 11; i++)
		{
			sem_post(&southStuff);
			pthread_sleep(1);
			//cout << "south asleep for " << i << "seconds" << endl;
			sem_wait(&southStuff);
		}
		
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
