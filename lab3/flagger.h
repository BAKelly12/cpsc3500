/**
 *@file: flagger.h - Header file for flagger class -- Basic thread manager
  *@Author: Brigid Kelly
  *@See:  "Seattle University CPSC 3500 Winter 2019"
  */
  
  #include <queue>
  #include <fstream>
  #include <pthread.h>
  #include <semaphore.h>
  #include <iostream>
	#include <string>
  
  #pragma once
  
  #define FLAGGER_LOG_FILENAME "flagger.log"
  
class flagger{
    
public:
    
     /**  Big Five **/
      flagger() = delete;
      ~flagger()= default;
      flagger(const void* (*ca)(void *args), std::string tln);//use sem_init for writers and readers
      flagger(const flagger& other) = default;
      flagger(flagger&& temp) = default;
	    flagger& operator=(const flagger& other) = default;
    	flagger& operator=(flagger&& temp) = default;
      
      /**Member Functions**/

      //getters
      int getp();
      int getc();
      
      
      //adds a consumer thread
      void* addc();
      
      
      //adds a producer thread
      void* addp();

      //This is also like a "go" command
      void* consume();
      
     
      //this is like a "go!" command
      void* produce();
      

private:
    
    /*under construction**
      struct threadInfo{//have each car fill out a small questionaire
        
        
      
        
      };
      
      */  
      
      const std::string selflog = FLAGGER_LOG_FILENAME;
      std::string tlog;
      
      std::queue<pthread_t> consQ;
      std::queue<pthread_t> prodQ;
      const void* (*critSect)(void* args);
      sem_t producer;
      sem_t consumer;

 
};//end class declaration