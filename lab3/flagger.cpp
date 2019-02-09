
/**
 *@file flagger.cpp - Implementation of flagger class
  *@authors Brigid Kelly/Sam VanNes
  *@see "Seattle University CPSC-3500 WINTER 2019"
  */
  
  
#include "flagger.h"
using namespace std; 
  
flagger::flagger(const void* (*ca)(void* args), string tln){
  
  tlog = tln;
  critSect = ca; 
  
  sem_init(&consumer, 0, 0);
  sem_init(&producer, 0, 0);
  
}

void* flagger::consume(){
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

int getc(){
	return 1;
}


int getp(){
	return 1;
}

