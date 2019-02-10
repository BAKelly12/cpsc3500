/**
 *@file: flagger.h - Header file for flagger class -- Basic thread manager
  *@Author: Brigid Kelly
  *@See:  "Seattle University CPSC 3500 Winter 2019"
  */
  
//#include <ctime>
#include <time.h>
#include <queue>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <string>
#include <iomanip>
  
#pragma once
  
#define FLAGGER_LOG_FILENAME "~/cpsc3500/lab3/flagger.log"
  
class flagger{
    
public:
    
/*Specific Constructor*/
     flagger(void* (*ca)(void *args), std::string tln);
     
     
/**  Big Six **/
      flagger() = delete;
      ~flagger()= default;
      flagger(const flagger& other) = default;
      flagger(flagger&& temp) = default;
	    flagger& operator=(const flagger& other) = default;
    	flagger& operator=(flagger&& temp) = default;
      
/** Member Functions **/

      //getters
      int getp();
      int getc();
      
      //Setters
      void* addc();
      void* addp();

      //Go commands
      void* consume();
      void* produce();
      

private:
    
  
 /** Private Member Functions **/
 
  std::string getTime()
  {
    std::time_t tanD = std::time(nullptr);
    char buff[20];
    struct std::tm *sTm = localtime(&tanD); 
    strftime(buff, sizeof(buff), "%H:%M:%S", sTm);   
    std::string now(buff);   
    return now; 
  }

  
  /**Private Member Variables**/

  /**under construction**
    
    
      struct threadInfo{//have each car fill out a small questionaire
    
     
      };
      
      */  
 
      std::string tlog;
      
      std::queue<pthread_t> consQ;
      std::queue<pthread_t> prodQ;
      
      void* (*critSect)(void* args);
      
      
      sem_t producer;
      sem_t consumer;
      
      
    
      

inline void fLogger( std::string logMsg ){

    std::string filePath = (std::string)FLAGGER_LOG_FILENAME;
    std::cout << (std::string)FLAGGER_LOG_FILENAME<<"\t";
    std::string now = getTime();
    
 
    std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << logMsg << '\n';
    ofs.close();
}


/**
inline void tLogger( string logMsg ){
 
    string now = getTime();
    ofstream ofs(tlog.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << logMsg << '\n';
    ofs.close();
}
*/
};//end class declaration