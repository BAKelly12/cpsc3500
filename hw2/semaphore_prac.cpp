/**
 * @file semaphore_prac.cpp - Basic demonstration of the use of
 *                          pthread and semaphores.
 * @author: Yingwu Zhu, edited by Brigid Kelly for functionality
 * @see: "Seattle University CPSC3500 Winter 2019"
 * */

#include <semaphore.h>
#include <iostream>
#include <pthread.h>
 
using namespace std;

sem_t oxygen, hydrogen;	// two Pthread semaphores

void *Oxygen (void *arg);
void *Hydrogen (void *arg);
 
int main (int argc, char *argv[])
{
  
  sem_init (&oxygen, 0, 0);
  sem_init (&hydrogen, 0, 0);
  const int num_of_water = 10;	//# of water molecules
  
  pthread_t tidox[num_of_water], tidhydro[2 * num_of_water];
  
    //create hydrogen atom threads
  for (int k = 0; k < num_of_water; k++)
      pthread_create (&tidox[k], NULL, Oxygen, NULL);
  
    //create hydrogen atom threads
  for (int k = 0; k < 2 * num_of_water; k++)
      pthread_create (&tidhydro[k], NULL, Hydrogen, NULL);
  
  for (int k = 0; k < 2 * num_of_water; k++)
      pthread_join (tidhydro[k], NULL);
  
  for (int k = 0; k < num_of_water; k++)
      pthread_join (tidox[k], NULL);
  
 
  sem_destroy (&hydrogen);
  sem_destroy (&oxygen);
  
  return 0;
}

void *Oxygen (void *arg)
{
    /* Post twice to allow 2 H through */
    sem_post(&oxygen);
    sem_post(&oxygen);
   
    return NULL;
}

 
void* Hydrogen (void *arg)
{
    int noxy(0);
    int nhy(0);
    int woxy(0);
    sem_getvalue(&oxygen, &woxy);
    
    /*wait for oxygen*/  
    while(1>woxy){}
       
    sem_wait(&oxygen);

    sem_getvalue(&oxygen, &nhy);
    cout<<"Hydrogen remaining: " << nhy << "\n"; 
        
    
    sem_getvalue(&oxygen, &noxy);
    cout<< "Oxygen remaining: "<< noxy/2 <<"\n";
   
    return NULL;
}

