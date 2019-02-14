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
bool entered = false;

sem_t northSendStuff, southSendStuff, southReturnStuff, northReturnStuff;
pthread_t makeCN, makeCS, critThread;

int main()
{
	
	sem_init(&northSendStuff, 0, 0);
	sem_init(&southReturnStuff, 0, 0);
	sem_init(&northSendStuff, 0, 0);
	sem_init(&southReturnStuff, 0, 0);

	pthread_mutex_init(&northVar, NULL);
	pthread_mutex_init(&southVar, NULL);
	
	
	pthread_create(&makeCN, NULL, makeCarsN, NULL);
	pthread_create(&makeCS, NULL, makeCarsS, NULL);
	pthread_create(&critThread, NULL, criticalSection, NULL);

	// *NOTE* The makeCN and makeCS threads are removed with detach().	
	//pthread_join(makeCN, NULL);
	//pthread_join(makeCS, NULL);
	pthread_join(critThread, NULL);

	pthread_mutex_destroy(&northVar);
	pthread_mutex_destroy(&southVar);

	sem_destroy(&northSendStuff);
	sem_destroy(&southSendStuff);
	sem_destroy(&northReturnStuff);
	sem_destroy(&southReturnStuff);
}

//Critical part of the process.
void* criticalSection(void* args)
{
	while (numCarsThru != numInteractions)
	{
		sem_wait(&northSendStuff);
		sem_wait(&southSendStuff);
			
		if (burstCarsN == 0 && burstCarsS == 0){
			cout << "flagger is asleep, number: " << numCarsThru << endl;
			pthread_sleep(1);
			numCarsThru++;
			sem_post(&northReturnStuff);
			sem_post(&southReturnStuff);
		}
		else if (burstCarsN > 0 && burstCarsS < 10){
			while (burstCarsN > 0 && burstCarsS < 10 && 
						 numCarsThru != numInteractions){
				
				if (entered == true) {
					sem_wait(&northSendStuff);
					sem_wait(&southSendStuff);
				}
				entered = true;
				pthread_mutex_lock(&northVar);
				burstCarsN--;
				pthread_mutex_unlock(&northVar);
				cout << "Car north goes thru number: " << numCarsThru << endl;
				pthread_sleep(1);
				numCarsThru++;
				sem_post(&northReturnStuff);
				sem_post(&southReturnStuff);
			}
			entered = false;
		}
		else if (burstCarsS > 0 && burstCarsN < 10){
			while (burstCarsS > 0 && burstCarsN < 10 && 
						 numCarsThru != numInteractions){
				
				if (entered == true) {
					sem_wait(&northSendStuff);
					sem_wait(&southSendStuff);
				}
				entered = true;
				pthread_mutex_lock(&southVar);
				burstCarsS--;
				pthread_mutex_unlock(&southVar);

				cout << "Car south goes thru number: " << numCarsThru << endl;
				pthread_sleep(1);
				numCarsThru++;
				sem_post(&northReturnStuff);
				sem_post(&southReturnStuff);
			}
			entered = false;
		}
		//Past this point, 10 or more cars are waiting on the car side.
		else {
			if (burstCarsN > 10) { //North more than 10 first
				cout << "North more than 10:";
				cout << endl << "CarsN: " << burstCarsN << endl;
				cout << "CarsS: " << burstCarsS << endl;
				entered = true; //Technically it is in north side again
				pthread_mutex_lock(&northVar);
				burstCarsN--;
				pthread_mutex_unlock(&northVar);
				cout << "Car north goes thru number: " << numCarsThru << endl;
				pthread_sleep(1);
				numCarsThru++;
				sem_post(&northReturnStuff);
				sem_post(&southReturnStuff);
				entered = false;
			}
			else { //South cars more than 10
				cout << "South more than 10:";
				cout << endl << "CarsN: " << burstCarsN << endl;
				cout << "CarsS: " << burstCarsS << endl;
				entered = true; //Technically it is in north side again
				pthread_mutex_lock(&southVar);
				burstCarsS--;
				pthread_mutex_unlock(&southVar);
				cout << "Car south goes thru number: " << numCarsThru << endl;
				pthread_sleep(1);
				numCarsThru++;
				sem_post(&northReturnStuff);
				sem_post(&southReturnStuff);
				entered = false;
			}
		}
	}
	//Get the other threads out
	sem_wait(&northSendStuff);
	sem_wait(&southSendStuff);
	pthread_detach(makeCN);
	pthread_detach(makeCS);

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
			sem_post(&northSendStuff);
			pthread_mutex_lock(&northVar);
			burstCarsN++;
			pthread_mutex_unlock(&northVar);
			pthread_sleep(1);
			sem_wait(&northReturnStuff);
		}
		for (int i = 0; i < 20; i++)
		{
			sem_post(&northSendStuff);
			pthread_sleep(1);
			//cout << "north asleep for " << i << "seconds" << endl;
			sem_wait(&northReturnStuff);
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
			sem_post(&southSendStuff);
			pthread_mutex_lock(&southVar);
			burstCarsS++;
			pthread_mutex_unlock(&southVar);
			pthread_sleep(1);
			sem_wait(&southReturnStuff);
		}
		for (int i = 0; i < 20; i++)
		{
			sem_post(&southSendStuff);
			pthread_sleep(1);
			//cout << "south asleep for " << i << "seconds" << endl;
			sem_wait(&southReturnStuff);
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
