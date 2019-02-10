
/**
 *@file flagger.cpp - Implementation of flagger class
  *@authors Brigid Kelly
  *@see "Seattle University CPSC-3500 WINTER 2019"
  */
  
  
#include "flagger.h"
using namespace std; 
  
flagger::flagger(void* (*ca)(void* args), string tln){
  
  tlog = tln;
  critSect = ca; 
  
  sem_init(&consumer, 0, 0);
  sem_init(&producer, 0, 0);
  
  string buff = getTime();
  
  printf("%s %s\n", buff.c_str(), "Flagger on scene\n");
  fLog("Awake");


}

void* flagger::consume(){

  pthread_t consTest;
  pthread_create (&consTest, NULL, critSect, NULL);
  
  pthread_join (consTest, NULL);
  
	return NULL;
}

void* flagger::produce(){
	return NULL;
}

void* flagger::addp(){
	return NULL;
}

void* flagger::addc(){
	return NULL;
}

int flagger::getc(){
	return 1;
}


int flagger::getp(){
	return 1;
}


