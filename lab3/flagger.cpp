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
  string buff = getTime();  
  direction = ' ';
  fLogFilePath = fln;
  tlog_enable = true;
  flog_enable = true;
  tLogHasHeader = false;
  fLogHasHeader = false;  
}


flagger::flagger(void* (*ca)(void* args), string fln, string tln, char dir){ 
  waiting = 0;
  tLogFilePath = tln;
  critSect = ca;  
  sem_init(&sem, 0, 0);
  string buff = getTime();  
  direction = toupper(dir);
  fLogFilePath = fln;
  tlog_enable = true;
  flog_enable = true;
  tLogHasHeader = false;
  fLogHasHeader = false;
  
}

flagger::~flagger(){
  sem_destroy(&sem);
}

int flagger::fLog_init(){
  if(!fLogHasHeader){
    flog_header();
    return 1;
  }else
    return -1;
}

int flagger::tLog_init(){
  if(!tLogHasHeader){
    tlog_header();
    return 1;
  }else
    return -1; 
}


void* flagger::create(){

   pthread_create (&consTest, NULL, critSect, NULL);
   return NULL;
   
}

void* flagger::wait(){//wait
  
  ++waiting;
  sem_wait(&sem);

	return NULL;
}

void* flagger::post(){//post

  sem_post(&sem);
 
	return NULL;
}

void* flagger::join(){ 
  pthread_join (consTest, NULL);
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
  
