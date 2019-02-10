/**
 *@file flagger.cpp - Implementation of flagger class
  *@authors Brigid Kelly
  *@see "Seattle University CPSC-3500 WINTER 2019"
  */
  
  
#include "flagger.h"

using namespace std; 
  
flagger::flagger(void* (*ca)(void* args), string fln, string tln){
  waiting = 0;
  tLogFilePath = tln;
  critSect = ca;  
  sem_init(&sem, 0, 0);
  direction = ' ';
  fLogFilePath = fln;
  fLog_init();
  tLog_init(); 
  tLogHasHeader = false;
  fLogHasHeader = false;  
  threadsInMotion = false;
}


flagger::flagger(void* (*ca)(void* args), string fln, string tln, char dir){ 
  waiting = 0;
  tLogFilePath = tln;
  critSect = ca;  
  sem_init(&sem, 0, 0);
  direction = ' ';
  fLogFilePath = fln;
  fLog_init();
  tLog_init(); 
  tLogHasHeader = false;
  fLogHasHeader = false; 
  threadsInMotion = false;
  
}

flagger::~flagger(){
  sem_destroy(&sem);
}

void* flagger::make_t(int num){
  if(!threadsInMotion)
    T_Q.resize(num);
  else
    cerr << "Cannot change thread queue size while threads are open\n";
  return NULL;
  
}

void* flagger::push_t(){
  
  if(!threadsInMotion)
    T_Q.resize(T_Q.size() + 1 );
  else
    cerr << "Cannot change thread queue size while threads are open\n";
  
  return NULL;
}
void* flagger::pop_t(){
  
  if(!threadsInMotion)
    T_Q.pop_back(); 
  else
    cerr << "Cannot change thread queue size while threads are open\n";
  
  return NULL;
}
  

void* flagger::create_t(){
  
  if(!threadsInMotion){ 
    for(size_t i(0); i<T_Q.size(); i++)
      pthread_create(&T_Q[i], NULL, critSect, NULL); 
    
    threadsInMotion = true;    
  }
  else
    cerr << "Cannot create more threads while threads are in motion";
  
  return NULL; 
}

void* flagger::wait(){
  //log arrival time here
  ++waiting;
  sem_wait(&sem);

	return NULL;
}

void* flagger::post(){
  //log critical section entry time here
  sem_post(&sem);
  --waiting;
 
	return NULL;
}

void* flagger::join(){ 

  for(size_t i(0); i<T_Q.size(); i++)
    pthread_join(T_Q[i], NULL); 
  //logging for vehicle exit here
  
  return NULL;
  
}

int flagger::getw(){
  return waiting;
}


int flagger::getp(){ 
  int n(0);
  sem_getvalue(&sem, &n); 
	return n;
}


int flagger::sleep(int seconds){
  
  fLog("Sleep");
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
  
  fLog("Awake");
  
  return pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
 
} 
